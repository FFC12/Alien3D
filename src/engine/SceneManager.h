#pragma once
#include <Base.hpp>
#include <map>
#include <unordered_map>
#include <pugixml.hpp> 
#include <utils/Uuid.h>

class GameObject;

class Scene {
public:
  explicit Scene(const std::string& sceneName) : m_SceneName(sceneName) {
    this->m_UUID = Utils::getUUID();
    this->m_ObjectID = sceneName + ":" + this->m_UUID;
    m_Root = m_Document.append_child("Scene"); 
    m_Root.append_attribute("name") = this->m_ObjectID.c_str(); 
  }

  void write(const GameObject&);
  void writeComponent(const std::string& nodeName, const std::map<std::string, std::string>& values); 

  void saveFile(const std::string& path);
  void readFromFile(const std::string& path);

  void resetData();
private:  
  std::string m_SceneName;
  std::string m_UUID;
  std::string m_ObjectID;

  pugi::xml_document m_Document;
  pugi::xml_node m_LastComponentsNode;
  pugi::xml_node m_Root;
};

class SceneManager {
public:
  static SceneManager& getInstance() {
    static SceneManager ins;
    return ins;
  }

  ~SceneManager() {
    for (auto& scene : Scenes) {
      delete scene.second;
    }
  }

  Scene* createScene(const std::string& sceneName) {
    Scenes[sceneName] = new Scene(sceneName);
    return Scenes[sceneName];
  }

  void saveScene(const std::string& sceneName, const std::string& path);
  void loadScene(const std::string& sceneName, const std::string& path);

private:
  SceneManager() {

  }

  static inline std::unordered_map<std::string, Scene*> Scenes{};
};