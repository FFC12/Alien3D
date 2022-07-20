#include <engine/GameObject.h>
#include <engine/PhysicsBody.h>
#include <engine/SpriteAnimation.h>
#include <Application.h>
#include "SceneManager.h"

void Scene::write(const GameObject& gameObject)
{
  auto child = m_Root.append_child("GameObject");
  
  child.append_attribute("name") = gameObject.m_Name.c_str();
  child.append_attribute("object_uuid") = gameObject.m_ObjectUUID.c_str();
  child.append_attribute("is_sprite") = gameObject.m_IsSprite;
  if (gameObject.m_IsSprite) {
    child.append_attribute("image_path") = gameObject.m_Texture->getImageRelativePath().c_str();
  }
  
  m_LastComponentsNode = child.append_child("Components");

  for (auto& component : gameObject.m_Components) {    
    component.second.first->OnSceneSerialized(*this);
  }
}

void Scene::writeComponent(const std::string& nodeName, const std::map<std::string, std::string>& values)
{
  auto node = m_LastComponentsNode.append_child(nodeName.c_str());
  for (auto& item : values) {
    node.append_attribute(item.first.c_str()) = item.second.c_str();
  }
}

void Scene::saveFile(const std::string& path)
{
  this->m_Document.save_file(path.c_str());
} 

void Scene::readFromFile(const std::string& path)
{
  this->m_Document.reset();
  auto result = this->m_Document.load_file(path.c_str());

  if (!result) {
    ALIEN_ERROR("Scene could not loaded!");
  }
  else {
    pugi::xml_node scene = this->m_Document.child("Scene");
    auto sceneUniqueName = scene.attribute("name").value();
    
    for (auto gameObject: scene) {
      auto name = gameObject.attribute("name").value();
      
      std::string isSpriteValue = gameObject.attribute("is_sprite").value();
      auto isSprite = false;
      
      if (isSpriteValue == "true") isSprite = true; else isSprite = false;

      if (isSprite) {
        auto path = gameObject.attribute("image_path").value();
        Sprite* sprite = new Sprite(name,path,true);

        auto transform = sprite->getComponent<Transform>("transform");
        auto texture = sprite->getComponent<Texture>("texture");
        auto shader = sprite->getComponent<Shader>("shader");

        auto components = gameObject.child("Components");
        for (auto component: components) {
          std::string componentName = component.name();
          if (componentName == "transform") {
            auto scaleX = component.attribute("scaleX").value();
            auto scaleY = component.attribute("scaleY").value();
            auto scaleZ = component.attribute("scaleZ").value();

            auto positionX = component.attribute("positionX").value();
            auto positionY = component.attribute("positionY").value();
            auto positionZ = component.attribute("positionZ").value();

            auto rotationX = component.attribute("rotationX").value();
            auto rotationY = component.attribute("rotationY").value();
            auto rotationZ = component.attribute("rotationZ").value();

            transform->setPosition(Vector3(std::strtold(positionX, nullptr),
              std::strtold(positionY, nullptr),
              std::strtold(positionZ, nullptr)));

            transform->setScale(Vector3(std::strtold(scaleX, nullptr), 
              std::strtold(scaleY, nullptr), 
              std::strtold(scaleZ, nullptr)));

            transform->setRotation(Vector3(std::strtold(rotationX, nullptr), std::strtold(rotationY, nullptr), std::strtold(rotationZ, nullptr)));
          } else if (componentName == "texture") {
            auto imagePath = component.attribute("imagePath").value();
            texture->setImagePath(imagePath);
            texture->generateTexture(imagePath, 0, "_Diffuse", false);
          } else if (componentName == "shader") {
            // nothing to do rigt now...
          }
          else {
            sprite->attachComponent(componentName); 
            if (componentName == "physics_body") {
              auto angle = component.attribute("angle").value();
              auto boundsX = component.attribute("boundsX").value();
              auto boundsY = component.attribute("boundsY").value();
              auto boundsZ = component.attribute("boundsZ").value();
              auto boundsCenterX = component.attribute("boundsCenterX").value();
              auto boundsCenterY = component.attribute("boundsCenterY").value();
              auto boundsCenterZ = component.attribute("boundsCenterZ").value();
              auto bodyType = component.attribute("bodyType").value();

              auto physicsBody = sprite->getComponent<PhysicsBody>("physics_body");

              physicsBody->setAngle(std::strtold(angle,nullptr));
              physicsBody->setBounds(Vector3(std::strtold(boundsX, nullptr), 
                                             std::strtold(boundsY, nullptr), 
                                             std::strtold(boundsZ, nullptr)));
              
              physicsBody->setBoundsCenter(Vector3(std::strtold(boundsCenterX, nullptr),
                                                   std::strtold(boundsCenterY, nullptr),
                                                   std::strtold(boundsCenterZ, nullptr)));

              physicsBody->setBodyType((BodyType)std::strtol(bodyType,nullptr,10));
            }
            else if (componentName == "sprite_animation") {
              auto H = std::strtol(component.attribute("H").value(), nullptr, 10);
              auto W = std::strtol(component.attribute("W").value(), nullptr, 10);
              auto X = std::strtold(component.attribute("X").value(), nullptr);
              auto Y = std::strtold(component.attribute("Y").value(), nullptr);
              auto playbackSpeed = std::strtold(component.attribute("playbackSpeed").value(), nullptr);

              auto spriteAnimation = sprite->getComponent<SpriteAnimation>("sprite_animation");
              spriteAnimation->setFrameX(X);
              spriteAnimation->setFrameY(Y);
              spriteAnimation->setHeight(H);
              spriteAnimation->setWidth(W);
              spriteAnimation->setPlaybackSpeed(playbackSpeed);
            }
          }
        }
      }
    }
  }
}

void Scene::resetData()
{
  m_Document.reset();
}

//-- SceneManager

void SceneManager::saveScene(const std::string& sceneName, const std::string& path)
{ 
  auto scene = Scenes[sceneName];
  for (auto& g : GameObject::GameobjectList) {
    scene->write(*g.second);
  }

  scene->saveFile(path);
  scene->resetData();
}

void SceneManager::loadScene(const std::string& sceneName, const std::string& path)
{
  auto scene = new Scene(sceneName);
  scene->readFromFile(path);
  Scenes[sceneName] = scene;
  AlienApplication::ActiveScene = scene;
}
