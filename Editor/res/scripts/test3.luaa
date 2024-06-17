local script = Karen.Script.new()
local movment

--local function lerp(on , dest, speed)
--  if(on == dest) then 
 --     return dest
  --elseif on > dest then
    --  on = on - speed;
     -- return on;
 -- elseif on < dest then 
   --   on = on + speed;
    --  return on;
    --end
 -- end

function script:onCreate()
  print("on create called")
  -- add the components 
  -- if not sure that it alredy has the component 
  -- use tryAdd<Component>()
  local circle_renderer = script.entity:addCircle()
  local body = script.entity:addRigidBody2D()
  local collider = script.entity:addCircleCollider()
  movment = script.entity:addMovment()

  -- set the circle color 
  circle_renderer.color = Karen.Vec4.new(1)

  print("on create end")
end

function script:onUpdate()
  print("on update called")
  local speed = 5
  if(Karen.Input.isKeyPressed(Karen.Keyboard.W)) then 
  movment.linear_velocity.y = speed
  elseif(Karen.Input.isKeyPressed(Karen.Keyboard.S)) then   
  movment.linear_velocity.y = -speed
--  else linear_velocity.y = lerp(linear_velocity.y, 0, speed); 
  end
if(Karen.Input.isKeyPressed(Karen.Keyboard.D)) then 
  movment.linear_velocity.x = speed
  elseif(Karen.Input.isKeyPressed(Karen.Keyboard.A)) then   
  movment.linear_velocity.x = -speed
  --else linear_velocity.x = lerp(linear_velocity.x, 0, speed); 
  end

print("on update end")
end

function script:onEnd()
print("on end")
end

function GetObject()
  print("getObject called")
  return script
end
