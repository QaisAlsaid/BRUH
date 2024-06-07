local script = Karen.Script.new()
local entity = script.entity
local num = 0


function script:onCreate()
  print("created entity id: ", entity:getId())
end


function script:onUpdate()
  local trans = entity:getTransform()
  local rot = trans.rotation
  rot.z = rot.z + num --script2:getTimestep()
  num = num + 0.001
end

function script:onDestroy()
end

function GetObject()
  return script
end
