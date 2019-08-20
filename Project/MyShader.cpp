#include "MyShader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vShaderCode = ReadFile(vertexPath);
	std::string fShaderCode = ReadFile(fragmentPath);

	unsigned int vertex = CompileShader(vShaderCode.c_str(), GL_VERTEX_SHADER);
	unsigned int fragment = CompileShader(fShaderCode.c_str(), GL_FRAGMENT_SHADER);

	LinkShader(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::UseShader()
{
	glUseProgram(ID);
}

void Shader::SetFloat(const string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetBool(const string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMatrix(const string &name, glm::mat4 value) const
{
	unsigned int transLocation = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(value));
}

//Private Function---------------------------------------------------------------------------
string Shader::ReadFile(const char* path)
{
	ifstream file;
	string strCode;
	file.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		file.open(path);
		stringstream stream;
		stream << file.rdbuf();
		file.close();

		strCode = stream.str();
	}
	catch (ifstream::failure e)
	{
		cout << path << " file not successfully read!" << endl;
	}

	return strCode;
}

unsigned int Shader::CompileShader(const char* code, GLenum type)
{
	unsigned int shader;
	int success;
	char infoLog[512];

	shader = glCreateShader(type);

	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << type << "Shader compile failed!" << endl;
		cout << "Info Log is: " << infoLog << endl;
	}
	else
	{
		cout << "Shader compile success!" << endl;
	}

	return shader;
}

void Shader::LinkShader(unsigned int vertex, unsigned int fragment)
{
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "Shader link failed!" << endl;
		cout << "InfoLog is: " << infoLog << endl;
	}
	else
	{
		cout << "Shader link success!" << endl;
	}

	cout << "ID = " << ID << endl;
}