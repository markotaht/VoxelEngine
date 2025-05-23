#include "Window.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar* msg, const void* data)
{
	const char* _source;
	const char* _type;
	const char* _severity;

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		_source = "API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		_source = "WINDOW SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		_source = "SHADER COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		_source = "THIRD PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		_source = "APPLICATION";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		_source = "UNKNOWN";
		break;

	default:
		_source = "UNKNOWN";
		break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		_type = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		_type = "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		_type = "UDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		_type = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		_type = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		_type = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		_type = "MARKER";
		break;

	default:
		_type = "UNKNOWN";
		break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		_severity = "HIGH";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		_severity = "MEDIUM";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		_severity = "LOW";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		_severity = "NOTIFICATION";
		break;

	default:
		_severity = "UNKNOWN";
		break;
	}

	printf("%d: %s of %s severity, raised from %s: %s\n",
		id, _type, _severity, _source, msg);
}

bool Window::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create window
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
	SDL_Window* createdWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (createdWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);
	window = std::move(WindowPtr(createdWindow, SDL_DestroyWindow));
	gContext = SDL_GL_CreateContext(window.get());
	if (gContext == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		return false;
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugMessageCallback, 0);
	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}


	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL image could not be initilaized! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window.get(), gContext);
	ImGui_ImplOpenGL3_Init("#version 440");
	return true;
}

void Window::update() {
	SDL_GL_SwapWindow(window.get());
	//SDL_UpdateWindowSurface(window);
}

Window::~Window() {
	SDL_GL_MakeCurrent(window.get(), gContext);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_GL_DeleteContext(gContext);
	for (auto it : threadContexts) {
		SDL_GL_DeleteContext(it);
	}
	IMG_Quit();
	SDL_Quit();
}

SDL_GLContext Window::getThreadContext(int i)
{
	if (threadContexts.size() < i + 1) {
		threadContexts.push_back(SDL_GL_CreateContext(window.get()));
		SDL_GL_MakeCurrent(window.get(), gContext);
	}
	return threadContexts[i];
}

void Window::toggleMouseMode()
{
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(!SDL_GetRelativeMouseMode()));
}
