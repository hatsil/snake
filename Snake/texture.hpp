#pragma once

typedef unsigned int uint;

namespace snake {
	class Texture {
	public:
		Texture(const char* fileName);
		virtual ~Texture();

		void Bind() const;

	private:
		uint m_texture;
	};
}