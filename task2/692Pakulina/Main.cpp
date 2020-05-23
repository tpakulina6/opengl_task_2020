#include <Application.hpp>
#include <LightInfo.hpp>
#include <Mesh.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

//# define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING "kuku"

#include <iostream>

class SampleApplication : public Application
{
public:

    MeshPtr _relief;
    MeshPtr _marker;

    ShaderProgramPtr _shader;
    ShaderProgramPtr _markerShader;

    float _lr = 15.0f;
    float _phi = 7.5f;
    float _theta = 7.5f;

    LightInfo _light;

    TexturePtr _iceTex;
    TexturePtr _sandTex;
    TexturePtr _mramorTex;
    TexturePtr _snowTex;
    TexturePtr _maskTex;

    GLuint _samplerIce;
    GLuint _samplerSand;
    GLuint _samplerMramor;
    GLuint _samplerSnow;
    GLuint _samplerMask;

    void makeScene() override
    {
        Application::makeScene();

        _relief = makeRelief(8.0f);
        _relief->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));


        _marker = makeSphere(0.5f);

        _cameraMover = std::make_shared<ReliefCameraMover>();


        _shader = std::make_shared<ShaderProgram>("692PakulinaData2/shader.vert", "692PakulinaData2/shader.frag");
        _markerShader = std::make_shared<ShaderProgram>("692PakulinaData2/marker.vert", "692PakulinaData2/marker.frag");

        //Инициализация значений переменных освщения
        _light.position = glm::vec3(glm::cos(_phi) * glm::cos(_theta), glm::sin(_phi) * glm::cos(_theta), glm::sin(_theta)) * _lr;
        _light.ambient = glm::vec3(0.2, 0.2, 0.2);
        _light.diffuse = glm::vec3(0.8, 0.8, 0.8);
        _light.specular = glm::vec3(1.0, 1.0, 1.0);

        _iceTex = loadTexture("692PakulinaData2/ice.jpg");
        _sandTex = loadTexture("692PakulinaData2/sand.png");
        _mramorTex = loadTexture("692PakulinaData2/mramor.png");
        _snowTex = loadTexture("692PakulinaData2/snow.jpg");
        _maskTex = loadTexture("692PakulinaData2/mask.png");


        glGenSamplers(1, &_samplerIce);
        glSamplerParameteri(_samplerIce, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerIce, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerIce, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(_samplerIce, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenSamplers(1, &_samplerSand);
        glSamplerParameteri(_samplerSand, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerSand, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerSand, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(_samplerSand, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenSamplers(1, &_samplerMramor);
        glSamplerParameteri(_samplerMramor, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerMramor, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerMramor, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(_samplerMramor, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenSamplers(1, &_samplerSnow);
        glSamplerParameteri(_samplerSnow, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerSnow, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerSnow, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(_samplerSnow, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenSamplers(1, &_samplerMask);
        glSamplerParameteri(_samplerMask, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerMask, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(_samplerMask, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(_samplerMask, GL_TEXTURE_WRAP_T, GL_REPEAT);


    }

    void updateGUI() override
    {
        Application::updateGUI();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
        if (ImGui::Begin("MIPT OpenGL Sample", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);

            if (ImGui::CollapsingHeader("Light"))
            {
                ImGui::ColorEdit3("ambient", glm::value_ptr(_light.ambient));
                ImGui::ColorEdit3("diffuse", glm::value_ptr(_light.diffuse));
                ImGui::ColorEdit3("specular", glm::value_ptr(_light.specular));

                ImGui::SliderFloat("radius", &_lr, 0.1f, 40.0f);
                ImGui::SliderFloat("phi", &_phi, 0.0f, 4.0f * glm::pi<float>());
                ImGui::SliderFloat("theta", &_theta, 0.0f,4.0f * glm::pi<float>());
            }
        }
        ImGui::End();
    }

    void draw() override
    {

        //Получаем текущие размеры экрана и выставлям вьюпорт
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        glViewport(0, 0, width, height);

        //Очищаем буферы цвета и глубины от результатов рендеринга предыдущего кадра
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Подключаем шейдер
        _shader->use();


        _light.position = glm::vec3(glm::cos(_phi) * glm::cos(_theta), glm::sin(_phi) * glm::cos(_theta), glm::sin(_theta)) * _lr;
        glm::vec3 lightPosCamSpace = glm::vec3(_camera.viewMatrix * glm::vec4(_light.position, 1.0));

        _shader->setVec3Uniform("light.pos", lightPosCamSpace); //копируем положение уже в системе виртуальной камеры
        _shader->setVec3Uniform("light.La", _light.ambient);
        _shader->setVec3Uniform("light.Ld", _light.diffuse);
        _shader->setVec3Uniform("light.Ls", _light.specular);


        glBindSampler(0, _samplerIce);
        glActiveTexture(GL_TEXTURE0);
        _iceTex->bind();
        _shader->setIntUniform("iceTex", 0);

        glBindSampler(1, _samplerMramor);
        glActiveTexture(GL_TEXTURE1);
        _mramorTex->bind();
        _shader->setIntUniform("mramorTex", 1);

        glBindSampler(2, _samplerSnow);
        glActiveTexture(GL_TEXTURE2);
        _snowTex->bind();
        _shader->setIntUniform("snowTex", 2);

        glBindSampler(3, _samplerMask);
        glActiveTexture(GL_TEXTURE3);
        _maskTex->bind();
        _shader->setIntUniform("maskTex", 3);

        glBindSampler(4, _samplerSand);
        glActiveTexture(GL_TEXTURE4);
        _sandTex->bind();
        _shader->setIntUniform("sandTex", 4);


        {
            _shader->setMat4Uniform("viewMatrix", _camera.viewMatrix);
            _shader->setMat4Uniform("projectionMatrix", _camera.projMatrix);
            _shader->setMat4Uniform("modelMatrix", _relief->modelMatrix());
            _shader->setMat3Uniform("normalToCameraMatrix",
                                    glm::transpose(glm::inverse(glm::mat3(_camera.viewMatrix * _relief->modelMatrix()))));

            _relief->draw();
        }


        {
            _markerShader->use();

            _markerShader->setMat4Uniform("mvpMatrix", _camera.projMatrix * _camera.viewMatrix * glm::translate(glm::mat4(1.0f), _light.position));
            _markerShader->setVec4Uniform("color", glm::vec4(_light.diffuse, 1.0f));
            _marker->draw();
        }

        //Отсоединяем сэмплер и шейдерную программу
        glBindSampler(0, 0);
        glUseProgram(0);

    }
};

int main()
{
    SampleApplication app;
    app.start();

    return 0;
}
