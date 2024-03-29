#include <fstream>
#include <iostream>

#include "obj_loader.hpp"

snake::OBJModel::OBJModel(const std::string& fileName): OBJModel(fileName, 0) {}

snake::OBJModel::OBJModel(const std::string & fileName, size_t maxFaces) {
	hasUVs = false;
	hasNormals = false;
	
	std::ifstream file;
	file.open(fileName.c_str());

	std::string line;
	if (file.is_open()) {
		std::set<Triangle> trianglesSet;
		while (file.good()) {
			std::getline(file, line);

			uint lineLength = line.length();

			if (lineLength < 2)
				continue;

			const char* lineCStr = line.c_str();

			switch (lineCStr[0]) {
			case 'v':
				if (lineCStr[1] == 't')
					uvs.push_back(ParseOBJVec2(line));
				else if (lineCStr[1] == 'n') {
					normals.push_back(ParseOBJVec3(line));
					colors.push_back((normals.back() + glm::vec3(1, 1, 1))*0.5f);
				} else if (lineCStr[1] == ' ' || lineCStr[1] == '\t')
					vertices.push_back(ParseOBJVec3(line));
				break;
			
			case 'f':
				CreateOBJFace(line, trianglesSet);
				break;
			
			default: break;
			}
		}

		file.close();

		// Initialize triangles list
		triangles.assign(trianglesSet.begin(), trianglesSet.end());

		// Clrearing the temp set
		trianglesSet.clear();

		this->maxFaces = maxFaces > triangles.size() ? triangles.size() : maxFaces;

		// Make room for neighbors list
		neighbors.resize(vertices.size());

		std::set<Edge, LexicographicComp> tempEdgesLexSet;

		// Initialize neighbors list end tempEdgesLexSet
		for(Triangle& t : triangles) {
			t.forEach([&](OBJIndex& obj) {
				neighbors[obj.vertexIndex].insert(&t);
			});

			t.forEach([&](Edge&& e) {
				tempEdgesLexSet.insert(e);
			});
		}

		// Initialize the error matrices
		InitQ();

		// Initialize edges sets
		for(const Edge& e : tempEdgesLexSet) {
			Edge weightedEdge = GetWeightedEdge(e);
			edgesLex.insert(weightedEdge);
			edgesWeights.insert(weightedEdge);
		}
		tempEdgesLexSet.clear();

		if(this->maxFaces > 0)
			SimplifyMesh();

	} else
		std::cerr << "Unable to load mesh: " << fileName << std::endl;
}

void snake::OBJModel::InitQ() {
	// Create Q(s)
	qs.resize(neighbors.size());
	for(size_t i = 0; i < neighbors.size(); ++i) {
		qs[i] = glm::dmat4(0);
		for(Triangle* t : neighbors[i])
			qs[i] += makeQ(Plane(*t, *this));
	}
}

void snake::OBJModel::SimplifyMesh() {
	// Every iteration reduces 2 triangles
	for(size_t i = triangles.size(); i > maxFaces && !edgesWeights.empty();) {
		auto mostLightweightEdgeIt = edgesWeights.begin();
		Edge mostLightweightEdge = *mostLightweightEdgeIt;

		const OBJIndex& drop = mostLightweightEdge.drop;
		const OBJIndex& keep = mostLightweightEdge.keep;
		
		std::set<Triangle*> trianglesToTransfer;
		std::set<Edge, LexicographicComp> dropEdges;
		
		for(Triangle* t : neighbors[drop.vertexIndex]) {
			if(!t->isDeleted) {
				if (t->Contains(mostLightweightEdge)) {
					t->Delete();
					--i;
				} else {
					t->forEach([&](Edge&& e) {
						if (e.Contains(drop))
							dropEdges.insert(e);
					});

					trianglesToTransfer.insert(t->Replace(drop, keep));
				}
			}
		}

		neighbors[drop.vertexIndex].clear();
		edgesLex.erase(mostLightweightEdge);
		edgesWeights.erase(mostLightweightEdgeIt);

		std::set<Edge, LexicographicComp> keepEdges;
		for(const Edge& e : dropEdges) {
			auto weightedEdgeIt = edgesLex.find(e);
			edgesWeights.erase(*weightedEdgeIt);
			edgesLex.erase(weightedEdgeIt);
			keepEdges.insert(e.Replace(drop, keep));
		}

		dropEdges.clear();
		auto& keepedVertexTriangles = neighbors[keep.vertexIndex];
		for(auto it = keepedVertexTriangles.begin(); it != keepedVertexTriangles.end();) {
			Triangle *t = *it;
			if(t->isDeleted)
				it = keepedVertexTriangles.erase(it);
			else {
				t->forEach([&](Edge&& e) {
					if(e.Contains(keep))
						dropEdges.insert(e);
				});

				++it;
			}
		}

		for(const Edge& e : dropEdges) {
			auto weightedEdgeIt = edgesLex.find(e);
			edgesWeights.erase(*weightedEdgeIt);
			edgesLex.erase(weightedEdgeIt);
			keepEdges.insert(e);
		}

		UpdateQ(mostLightweightEdge);
		UpdateVertexPos(mostLightweightEdge);

		for(const Edge& e : keepEdges) {
			Edge weightedEdge = GetWeightedEdge(e);
			edgesLex.insert(weightedEdge);
			edgesWeights.insert(weightedEdge);
		}

		keepedVertexTriangles.insert(trianglesToTransfer.begin(), trianglesToTransfer.end());
	}
}

void snake::OBJModel::ToIndexedModel(IndexedModel& indexModel) {
	indexModel.Clear();

	auto& positions = indexModel.positions;
	auto& texCoords = indexModel.texCoords;
	auto& normals = indexModel.normals;
	auto& colors = indexModel.colors;
	auto& indices = indexModel.indices;

	std::list<Triangle*> remainingTriangles;
	std::set<OBJIndex> remainingVertices;
	
	for (Triangle& t : triangles)
		if (!t.isDeleted) {
			remainingTriangles.push_back(&t);
			t.forEach([&](OBJIndex& obj) {
				remainingVertices.insert(obj);
			});
		}

	CalcNormals(remainingVertices);
	size_t finalSize = remainingVertices.size();
	
	positions.resize(finalSize);
	texCoords.resize(finalSize);
	normals.resize(finalSize);
	colors.resize(finalSize);

	std::vector<uint> indicesMapping(vertices.size());
	uint i = 0U;
	for(const OBJIndex& obj : remainingVertices) {
		uint j = obj.vertexIndex;
		positions[i] = vertices[j];
		texCoords[i] = hasUVs ? uvs[obj.uvIndex] : glm::vec2(0,0);
		normals[i] = this->normals[j];
		colors[i] = this->normals[j]; // don't ask me why....
		indicesMapping[j] = i++;
	}

	indices.resize(remainingTriangles.size() * 3U);
	i = 0U;
	for(Triangle* t : remainingTriangles) {
		t->forEach([&](const OBJIndex& obj) {
			uint j = obj.vertexIndex;
			indices[i++] = indicesMapping[j];
		});
	}
}

void snake::OBJModel::CalcNormals(const std::set<OBJIndex>& remainingVertices) {
	if(normals.size() < vertices.size())
		normals.resize(vertices.size());

	for(const OBJIndex& obj : remainingVertices) {
		glm::vec3 normal(0,0,0);
		int numOfTriangles = 0;
		for(Triangle* t : neighbors[obj.vertexIndex]) {
			if(!t->isDeleted) {
				++numOfTriangles;
				uint i0 = t->a.vertexIndex;
				uint i1 = t->b.vertexIndex;
				uint i2 = t->c.vertexIndex;

				normal += MakeNormal(i0, i1, i2);
			}
		}

		if(numOfTriangles) {
			float avg = 1.0f / (float)numOfTriangles;
			normal = normal == glm::vec3(0,0,0) ? normal : glm::normalize(avg*normal);
		}

		normals[obj.vertexIndex] = normal;
	}
}