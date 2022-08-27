BehaviorDesigner.ParentTask = class("ParentTask",BehaviorDesigner.Task)
local ParentTask = BehaviorDesigner.ParentTask

function ParentTask:ctor()
    self.children = {}
end

function ParentTask:MaxChildren()
    return self.children and #self.children or 0
end

function ParentTask:CanRunParallelChildren()
    return false
end

function ParentTask:CurrentChildIndex()
    return 0
end

function ParentTask:CanExecute()
    return true
end

function ParentTask:Decorate(status)
    return status
end

function ParentTask:CanReevaluate()
    return false
end


function ParentTask:OnChildExecuted(child_index,child_status)

end

function ParentTask:OnChildStarted(child_index)
    
end

function ParentTask:GetUtility()
    local utility = 0
    if self.children then
        
    end
end

function ParentTask:OverrideStatus()
    return BehaviorDesigner.TaskStatus.Running
end