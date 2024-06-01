script = Karen.Script.new()
local sprite;
local trans;
local asset_manager;
local scene;
local scene_camera_entity;
local body;
local karen = Karen.Vec4.new(1);
local col = Karen.Vec4.new(255);
--local qais = 1.0;

print("made the create")
function script:onCreate()
  script:export("Karen", karen)
  --script:export("Qais", qais)
  script:export("Color", col, true)
  --script:export("q", q)
  --script:export("k", k)


  asset_manager = Karen.App.getAssetManager();
  print(asset_manager)
  scene = asset_manager:getScene("scene")
  print("making entity")
  --e = Karen.Entity.new(3, scene);
  --print("entity id: ", e:getId());
  scene_camera_entity = scene:getEntity("Camera");
  print("onCreate Called")
  print("id", script.entity:getId());
  print("getting sprite")
  --sprite = Karen.Scene.Components.Sprite.new();
  sprite = script.entity:addSprite()
  body = script.entity:addRigidBody2D()
  local box = script.entity:addBoxCollider()

  trans = script.entity:getTransform()
  --print(script.entity:insertRigidBody2D())
  --box = script.entity:insertBoxCollider()
  --box.restitution = 1;
  print("first color: ", sprite.color);
  --print(script.entity:getUUID());
  --local sprite = script.entity:getSprite()
 -- print("sprite: ", sprite)
  print("done getting sprite")
 -- sprite.color = Karen.Math.Vec4.new();
  sprite.color.x = 0.1;
  sprite.color.y = 0.5;
  sprite.color.z = 0.1;
  sprite.color.w = 1.0;

  print("getting trans")
  --trans = script.entity:insertTransform()
  --trans.rotation = Karen.Vec4.new()
  print("done getting trans")

  --trans.position.x = Karen.Vec4.new()
  --trans.position.y = Karen.Vec4.new()
  --trans.rotation.z = 
  --print(vec1 + vec2)
  print("exited onCreate")
end
local num = 0

function script:onUpdate()
  --print("Scene*");
  print("===================")
  print("karen: ", karen)
  print("===================")
  --if(Karen.Input.isKeyPressed(87)) then
  --body.linear_velocity = Karen.Vec4.new(0, 3) end
  --if(Karen.Input.isKeyPressed(83)) then
  --body.linear_velocity = Karen.Vec4.new(0, -3) end
  --if(Karen.Input.isKeyPressed(65)) then
 -- body.linear_velocity = Karen.Vec4.new(3, 0) end
 -- if(Karen.Input.isKeyPressed(69)) then
 -- body.linear_velocity = Karen.Vec4.new(-3, 0) end

  --trans.position.x = math.sin(num) * 4
  --trans.position.y = math.cos(num) * 6
 -- trans.rotation.z = math.tan(num);

  local pos = Karen.Vec3.new(0, 0, -0.1)
  --local rot = Karen
  local size = Karen.Vec2.new(0, 0)
  local color = Karen.Vec4.new(0.3, 0.3, 0.8, 1.0)
--camera matrix 
  local cam_transform = Karen.Mat4.new(1);
  cam_transform = cam_transform:translate(Karen.Vec4.new(0))
  --
  --local _pos = pos:sub(Karen.Vec3.new(size.x, size.y, 1):dev(2))
--  local cam_proj = scene_camera_entity:getCamera().camera:getProjection()
--  local cam_trans = scene_camera_entity:getTransform():asMat4()
  --Karen.Renderer2D.beginScene(cam_proj, cam_trans)
  --Karen.Renderer2D.drawQuad(pos, size, color)
  --Karen.Renderer2D.endScene()
  local ts = script:getTimestep()
  sprite.color = col;
  --sprite.color.z = math.tan(num)
--  if not ts == nil then
    print(sprite.color)
    print(ts)
  --else print("ts is nill")
  --end
  --trans.rotation.z = trans.rotation.z + math.sin(num)


  num = num + 0.01;
  if num > 1 then
    --num = 0
  end
end

function script:onDestroy()
  --karen = "karen"
  --qais = "qais"
  print("onEnd")
  local v1 = Karen.Vec4.new(2)
  local v2 = Karen.Vec4.new(1)
  local m1 = Karen.Mat4.new(2)
  local m2 = Karen.Mat4.new(3)
  print(m1:mul(v2))
  print(v1:mul(v2))
  print(m1:translate(v1))
  print(v1:add(v2))
  print(m1:inverse())
end

print("made getobj")
function GetObject()
  print("GetObject Called")
  return script;
end
