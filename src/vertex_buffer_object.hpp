#ifndef VERTEX_BUFFER_OBJECT_HPP
#define VERTEX_BUFFER_OBJECT_HPP

#include "opengl.hpp"
#include <vector>

class VertexBufferObject
{
	public:
		enum Target
		{
			ArrayBuffer = GL_ARRAY_BUFFER,
			ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
			PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
			PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
		};

		enum Usage
		{
			StreamDraw = GL_STREAM_DRAW,
			StreamRead = GL_STREAM_READ,
			StreamCopy = GL_STREAM_COPY,
			StaticDraw = GL_STATIC_DRAW,
			StaticRead = GL_STATIC_READ,
			StaticCopy = GL_STATIC_COPY,
			DynamicDraw = GL_DYNAMIC_DRAW,
			DynamicRead = GL_DYNAMIC_READ,
			DynamicCopy = GL_DYNAMIC_COPY
		};

		VertexBufferObject(Target target)
		{
			_size = 0;
			_target = target;
			glGenBuffers(1, &_buffer_id);
		}

		~VertexBufferObject()
		{
			glDeleteBuffers(1, &_buffer_id);
		}

		template<typename T>
		void fill(const std::vector<T>& data, Usage usage);

		void bind()
		{
			glBindBuffer(_target, _buffer_id);
		}

		unsigned int size() const { return _size; }

	protected:
		unsigned int _buffer_id;
		unsigned int _size;
		Target _target;
};

template<typename T>
void VertexBufferObject::fill(const std::vector<T>& data, Usage usage)
{
	bind();
	const GLsizeiptr byte_size = sizeof(T) * data.size();
	_size = data.size();
	glBufferData(_target, byte_size, &data[0], usage);
}

#endif

