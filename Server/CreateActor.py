# coding=utf-8
import os,sys

path = os.getcwd() + "/"

CMakeTemplate = "add_library({0}Actor SHARED {0}Actor.hpp {0}Actor.cpp) \n\
target_link_libraries({0}Actor\n\
    ActorNet\n\
)\n\
SET_PROPERTY(TARGET {0}Actor PROPERTY FOLDER \"Server\")"

HeadTemplte = "#pragma once\n\
#include \"Actor.h\"\n\
#include \"ActorMessage.h\"\n\
\n\
using namespace actor_net;\n\
\n\
class {0}Actor : public Actor\n\
{{\n\
public:\n\
	{0}Actor();\n\
	virtual ~{0}Actor();\n\
\n\
	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;\n\
	void Stop() override;\n\
\n\
	void Receive(ActorMessage&& actor_msg) override;\n\
private:\n\
\n\
}};"
SourceTemplate = "#include \"{0}Actor.hpp\"\n\
\n\
{0}Actor::{0}Actor()\n\
{{\n\
\n\
}}\n\
\n\
{0}Actor::~{0}Actor()\n\
{{\n\
\n\
}}\n\
\n\
bool {0}Actor::Init(const std::shared_ptr<ActorNet>& actor_net)\n\
{{\n\
	if (!Actor::Init(actor_net))\n\
	{{\n\
		return false;\n\
	}}\n\
\n\
	std::cout << \"{0}Actor start!!!\" << std::endl;\n\
	return true;\n\
}}\n\
\n\
void {0}Actor::Stop()\n\
{{\n\
\n\
}}\n\
\n\
void {0}Actor::Receive(ActorMessage&& actor_msg)\n\
{{\n\
}}\n\
\n\
ACTOR_IMPLEMENT({0}Actor)"

def CreateActor(name):
    actor = name + "Actor"
    if not os.path.exists(path + actor):
        os.makedirs(path + actor)
        
        cmake_content = CMakeTemplate.format(name)
        head_content = HeadTemplte.format(name)
        source_content = SourceTemplate.format(name)

        f = open(path + actor + "/CMakeLists.txt","a")
        f.write(cmake_content)
        f.close()

        f = open(path + actor + "/{0}.hpp".format(actor),"a")
        f.write(head_content)
        f.close()

        f = open(path + actor + "/{0}.cpp".format(actor),"a")
        f.write(source_content)
        f.close()

        f = open(path + "CMakeLists.txt","a")
        f.write("add_subdirectory({0})\n".format(actor))
        f.close()
    else:
        print(actor + " exists")

def main():
    print(path)
    name = raw_input("enter actor name:")
    print(name)
    CreateActor(name)
if __name__ == "__main__":
    main()
