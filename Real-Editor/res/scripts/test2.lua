local script = Real.Script.new()
local entity = script.entity
local num = 0


function script:onCreate()
  print("created entity id: ", entity:getId())
end


function script:onUpdate()
  print("entity: ", entity)
  entity.addSprite(Real.Vec4.new(1, 1, 1, 1))
end

function script:onDestroy()
end

function GetObject()
  return script
end
