#include "Framework/Game/FileService.hpp"

class ServerFileService : public FileService
{
public:
	std::string OpenFile(std::string_view file) const override;
};

