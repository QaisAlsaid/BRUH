local script = Karen.Script.new()
local sprite;
local trans;

print("made the create")
function script:onCreate()
  print("onCreate Called")
  print("id", script.entity:getId());
  print("getting sprite")
  --sprite = Karen.Scene.Components.Sprite.new();
  sprite = script.entity:addSprite()
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
  trans.position.x = math.sin(num) * 4
  trans.position.y = math.cos(num) * 6
  trans.rotation.z = math.tan(num);

  local pos = Karen.Math.Vec3.new(0, 0, -0.1)
  --local rot = Karen.Math
  local size = Karen.Math.Vec2.new(1, 1)
  local color = Karen.Math.Vec4.new(0.3, 0.3, 0.8, 1.0)
--camera matrix 
  local cam_transform = Karen.Math.Mat4.new(1);
  cam_transform = cam_transform:translate(Karen.Math.Vec4.new(0))
  --
  Karen.Renderer2D.beginScene(Karen.Math.Mat4.new(1), cam_transform:inverse())
  Karen.Renderer2D.drawQuad(Karen.Math.Mat4.new(1), color)--(pos, size, color)
  Karen.Renderer2D.endScene()
  local ts = script:getTimestep()
  sprite.color.x = 0.7
  sprite.color.y = 0.2
  sprite.color.z = math.tan(num)
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
  print("onEnd")
  local v1 = Karen.Math.Vec4.new(2)
  local v2 = Karen.Math.Vec4.new(1)
  local m1 = Karen.Math.Mat4.new(2)
  local m2 = Karen.Math.Mat4.new(3)
  print(m1:mul(v2))
  print(v1:mul(v2))
  print(((m1:mul(m2)):inverse()):transpose())
  print(m1:translate(v1))
  print(v1:add(v2))
end

print("made getobj")
function GetObject()
  print("GetObject Called")
  return script;
end
