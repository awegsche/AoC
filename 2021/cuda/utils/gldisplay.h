#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <cuda_helpers.h>

class GlDisplay {

public:
	GlDisplay() {

    GLuint m_vertex_array;
    GL_CHECK( glGenVertexArrays(1, &m_vertex_array ) );
    GL_CHECK( glBindVertexArray( m_vertex_array ) );

	m_program = createGLProgram( s_vert_source, s_frag_source );
	m_render_tex_uniform_loc = getGLUniformLocation( m_program, "render_tex");

    GL_CHECK( glGenTextures( 1, &m_render_tex ) );
    GL_CHECK( glBindTexture( GL_TEXTURE_2D, m_render_tex ) );

    GL_CHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
    GL_CHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
    GL_CHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
    GL_CHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	GL_CHECK( glGenBuffers( 1, &m_quad_vertex_buffer ) );
	GL_CHECK( glBindBuffer( GL_ARRAY_BUFFER, m_quad_vertex_buffer ) );
	GL_CHECK( glBufferData( GL_ARRAY_BUFFER,
            sizeof( g_quad_vertex_buffer_data),
            g_quad_vertex_buffer_data,
            GL_STATIC_DRAW
            )
        );

    GL_CHECK_ERRORS();
	}
};