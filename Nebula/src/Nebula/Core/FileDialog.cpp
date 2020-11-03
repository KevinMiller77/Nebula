#include "FileDialog.h"

namespace Nebula
{
    std::string FileDialogs::OpenFile(const char* filter)
    {
		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_OpenDialog( filter, NULL, &outPath );

		if ( result == NFD_OKAY )
		{
			std::string out(outPath);
			free(outPath);
			return out;
		}
		else if ( result == NFD_CANCEL ) 
		{
		}
		else 
		{
			printf("Error: %s\n", NFD_GetError() );
		}

		return std::string();
    }

	std::string FileDialogs::SaveFile(const char* filter)
	{
		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_SaveDialog( filter, NULL, &outPath );

		if ( result == NFD_OKAY )
		{
			std::string out(outPath);
			free(outPath);
			return out;
		}
		else if ( result == NFD_CANCEL ) 
		{
		}
		else 
		{
			printf("Error: %s\n", NFD_GetError() );
		}

		return std::string();
	}
}