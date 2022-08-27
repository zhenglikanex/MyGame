BehaviorDesigner.Task = class("Task")
local Task = BehaviorDesigner.Task

function Task:ctor(owner)
    self.owner = owner
    self.id = -1
    self.friendly_name = ""
    self.is_instant = true
    self.refrence_id = -1
    self.disabled = false  
end

function Task:OnAwake()

end

function Task:OnStart()
end

function Task:OnUpdate()
end

function Task:OnEnd()
end

function Task:OnPause(pause)

end

function Task:GetPriority()
    return 0.0
end

function Task:GetUtility()
    return 0.0
end

function Task:OnBehaviorRestart()
end

function Task:OnBehaviorComplete()
end

function Task:OnReset()
end

