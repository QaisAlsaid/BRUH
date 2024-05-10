#include "SceneSerializer.h"
#include "Components.h"


namespace YAML
{
  template<>
  struct convert<Karen::Vec4>
  {
    static Node encode(const Karen::Vec4& vec)
    {
      Node n;
      n.push_back(vec.x);
      n.push_back(vec.y);
      n.push_back(vec.z);
      n.push_back(vec.w);
      return n;
    }

    static bool decode(const Node& node, Karen::Vec4& vec)
    {
      if(!node.IsSequence())
        return false;

      vec.x = node[0].as<float>();
      vec.y = node[1].as<float>();
      vec.z = node[2].as<float>();
      vec.w = node[3].as<float>();
      
      return true;
    }

  };

  template<>
  struct convert<Karen::Vec3>
  {
    static Node encode(const Karen::Vec3& vec)
    {
      Node n;
      n.push_back(vec.x);
      n.push_back(vec.y);
      n.push_back(vec.z);
      return n;
    }

    static bool decode(const Node& node, Karen::Vec3& vec)
    {
      if(!node.IsSequence())
        return false;

      vec.x = node[0].as<float>();
      vec.y = node[1].as<float>();
      vec.z = node[2].as<float>();
      
      return true;
    }
  };


  Emitter& operator << (Emitter& emitter, const Karen::Vec3& vec)
  {
    emitter << Flow;
    emitter << BeginSeq << vec.x << vec.y << vec.z << EndSeq;
    return emitter;
  }

  Emitter& operator << (Emitter& emitter, const Karen::Vec4& vec)
  {
    emitter << Flow;
    emitter << BeginSeq << vec.x << vec.y << vec.z << vec.w << EndSeq;
    return emitter;
  }
}


namespace Karen
{
  using namespace YAML;

  SceneSerializer::SceneSerializer(const ARef<Scene>& scene)
    : m_context(scene)
  {
  }

  void SceneSerializer::setContext(const ARef<Scene>& scene)
  {
    m_context = scene;
  }

  bool SceneSerializer::serializeText(const char* path)
  {
    Emitter emitter;
    emitter << BeginMap;
    
    emitter << Key << "Scene" << Value << m_context->m_name;

    emitter << Key << "Entities" << Value << BeginSeq;
    m_context->m_registry.view<TagComponent>().each([&](auto id, auto& tc)
    {
      Entity e(id, m_context.get());
      serializeEntity(e, emitter);
    });

    emitter << EndSeq << EndMap;

    std::ofstream fstream(path);

    fstream << emitter.c_str();

    return true;
  }

  bool SceneSerializer::deSerializeText(const char* path)
  {
    std::ifstream fstream(path);
    std::stringstream ss;
    if(!fstream.good())
    {
      KAREN_CORE_ERROR("Can't Load File: {0}", path);
      return false;
    }
    ss << fstream.rdbuf();
    KAREN_CORE_WARN("Scene data: {0}", ss.str());
    const auto& data = Load(ss.str().c_str());
    
    const auto& scene = data["Scene"];
    if(!scene)
    {
      KAREN_CORE_ERROR("No Scene Node in Scene Serializeation File !?");
      return false;
    }
    std::string scene_name = scene.as<std::string>();
    m_context = createARef<Scene>(scene_name);
    const auto& entities = data["Entities"];
    if(entities)
    {
      for(const auto& entity : entities)
      {
        //TODO: uint64_t uuid = entity["UUID"].as<uint64_t>();
        std::string tag;
        const auto& ctagn = entity["TagComponent"];
        if(ctagn)
          tag = ctagn["Tag"].as<std::string>();
        Entity e = m_context->addEntity(tag);
        const auto& ctransform_n = entity["TransformComponent"]; 
        if(ctransform_n)
        {
          auto& ctransform = e.insertComponent<TransformComponent>();
          ctransform.position = ctransform_n["Position"].as<Vec3>();
          ctransform.rotation = ctransform_n["Rotation"].as<Vec3>();
          ctransform.scale = ctransform_n["Scale"].as<Vec3>();
        }
        const auto& csprite_n = entity["SpriteComponent"];
        if(csprite_n)
        {
          auto& csprite = e.addComponent<SpriteComponent>();
          const auto& color = csprite_n["Color"].as<Vec4>();
          csprite.color = color / 255.0f;
          const auto& texture_handel = csprite_n["TextureHandel"].as<std::string>();
          csprite.texture_handel = texture_handel;
        }
        const auto& ccamera_n = entity["CameraComponent"];
        if(ccamera_n)
        {
          auto& ccamera = e.addComponent<CameraComponent>();
          auto& cam = ccamera.camera;
          const auto& pd_n = ccamera_n["PerspectiveData"];
          float p_fov = pd_n["FOV"].as<float>();
          float p_near_clip = pd_n["NearClip"].as<float>();
          float p_far_clip = pd_n["FarClip"].as<float>();
          cam.setPerspectiveData({p_fov, p_near_clip, p_far_clip});
          const auto& od_n = ccamera_n["OrthographicData"];
          float o_size = od_n["Size"].as<float>();
          float o_near_clip = od_n["NearClip"].as<float>();
          float o_far_clip = od_n["FarClip"].as<float>();
          cam.setOrthographicData({o_size, o_near_clip, o_far_clip});

          cam.setType((SceneCamera::ProjectionType)ccamera_n["Type"].as<int>());
          ccamera.is_primary = ccamera_n["Primary"].as<bool>();
          ccamera.is_fixed_aspect_ratio = ccamera_n["FixedAspectRatio"].as<bool>();
        }
      }
    }
    return true;
  }

  void SceneSerializer::serializeEntity(Entity& e, YAML::Emitter& emitter)
  {
    emitter << BeginMap;
    emitter << Key << "UUID" <<Value << "5345347092";

    if(e.hasComponent<TagComponent>())
    {
      emitter << Key << "TagComponent" << BeginMap;
      const auto& tag = e.getComponent<TagComponent>().name;
      emitter << Key << "Tag" << Value << tag;

      emitter << EndMap;
    }

    if(e.hasComponent<TransformComponent>())
    {
      emitter << Key << "TransformComponent" << BeginMap;

      auto& trans = e.getComponent<TransformComponent>();

      emitter << Key << "Position" << trans.position;
      emitter << Key << "Rotation" << trans.rotation;
      emitter << Key << "Scale" << trans.scale;

      emitter << EndMap;
    }

    if(e.hasComponent<SpriteComponent>())
    {
      emitter << Key << "SpriteComponent" << BeginMap;
      
      auto& sprite = e.getComponent<SpriteComponent>();

      emitter << Key << "Color" << Value << sprite.color * 255.0f;
      emitter << Key << "TextureHandel" << Value << sprite.texture_handel;
      emitter << EndMap;
    }

    if(e.hasComponent<CameraComponent>())
    {
      emitter << Key << "CameraComponent" << BeginMap;
      
      auto& cam = e. getComponent<CameraComponent>();

      const auto& pd = cam.camera.getPerspectiveData();
      emitter << Key << "PerspectiveData" << Value << BeginMap;
      emitter << Key << "FOV" << Value << pd.fov;
      emitter << Key << "NearClip" << Value << pd.near_clip;
      emitter << Key << "FarClip" << Value << pd.far_clip;
      emitter << EndMap;

      const auto& od = cam.camera.getOrthographicData();
      emitter << Key << "OrthographicData" << Value << BeginMap;
      emitter << Key << "Size" << Value << od.size;
      emitter << Key << "NearClip" << Value << od.near_clip;
      emitter << Key << "FarClip" << Value << od.far_clip;
      emitter << EndMap;
  
      emitter << Key << "Type" << Value <<(int)cam.camera.getType();

      emitter << Key << "Primary" << Value << cam.is_primary;
      emitter << Key << "FixedAspectRatio" << cam.is_fixed_aspect_ratio;
      
      emitter << EndMap;
    }

    emitter << EndMap;

  }
}
