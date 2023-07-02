#include "INIFile.h"

INIFile::INIFile()
{
	VALUES = std::map<std::string, std::string>();
	HEADERS = std::map<std::string, int>();
	PATH = "";
}

INIFile::INIFile(const char* _Path)
{
	VALUES = std::map<std::string, std::string>();
	HEADERS = std::map<std::string, int>();
	PATH = std::string(_Path);

	ValidateINIFile(PATH);
	LoadINIFileData(PATH);
}

INIFile::INIFile(std::string _Path)
{
	VALUES = std::map<std::string, std::string>();
	HEADERS = std::map<std::string, int>();
	PATH = std::string(_Path);

	ValidateINIFile(PATH);
	LoadINIFileData(PATH);
}

INIFile::INIFile(std::map<std::string, std::string> _VALUES)
{
	VALUES = _VALUES;
	HEADERS = std::map<std::string, int>();
	PATH = "";
}

INIFile::~INIFile()
{
	VALUES.clear();
	HEADERS.clear();
}

HRESULT INIFile::ValidateINIFile(std::string _Path)
{
	//SET PATH TO UPPERCASE LETTERS
	std::string P = std::string(_Path);
	std::transform(P.begin(), P.end(),
		P.begin(), ::toupper);

	//CREATE BUFFERS
	std::string E = ".INI";
	std::string C = "";

	//IF THE FILE IS LONGER
	//THAN THE EXTENSION
	if (P.size() > 4)
	{
		//GET THE SUBSTRING OF THE 
		//LAST 4 CHARACTERS
		C = P.substr(P.size() - 4, 4);

		//IF SUBSTRING MATCHES ENDING
		if (C == E)
		{
			//FILE IS INI FILE
			std::ifstream _Input(_Path.c_str());
			if (_Input.good())
			{
				//FILE EXISTS AND IS ACCESSIBLE
				_Input.close();
				return S_OK;
			}
			else
			{
				//FILE DOES NOT EXIST OR
				//CANNOT BE ACCESSED
				_Input.close();
				return E_INI_FILE_BAD_STREAM;
			}
		}
		else
		{
			//THE LAST 4 CHARACTERS OF THE
			//PATH DO NOT REPRESENT THE
			//CHARACTERS ".INI" OR THE
			//CURRENT VALUE OF E
			return E_INI_FILE_BAD_TYPE;
		}
	}

	//FILE PATH ISN'T LONG ENOUGH
	//TO CONTAIN FILE EXTENSION
	return E_INI_FILE_BAD_PATH;
}

HRESULT INIFile::LoadINIFileData(std::string _Path)
{
	//OPEN FILE AND CREATE BUFFER TO WRITE TO
	std::ifstream _Input(_Path.c_str(), std::ios::in);
	std::string _Buffer = "";

	//IF FILE WAS OPENED SUCCESSFULLY
	if (_Input.good())
	{
		//OPEN STRING STREAM AND READ ALL
		//FILE DATA THROUGH STRING STREAM
		//AND THEN COPY THE DATA INTO THE BUFFER
		std::ostringstream SS;
		SS << _Input.rdbuf();
		_Buffer = SS.str();

		//CLOSE FILE STREAM
		_Input.close();

		int _Index = 0;
		char CC = '#';
		int _Scan = 1;

		while (_Index < _Buffer.size() - 2)
		{
			//GET NEXT CHARACTER
			char C = _Buffer[_Index];

			switch (C)
			{
				//START OF A HEADER
				case '[':
					//FIND END OF HEADER
					while (CC != ']')
					{
						CC = _Buffer[_Index + _Scan];
						_Scan++;
					}
					
					HEADERS.emplace(_Buffer.substr(_Index, _Scan), _Index);

					//SET INDEX TO END
					_Index = _Index + _Scan + 1;
					break;

				case '\n':

					break;

				//NOT A HEADER, READ LINE AS
				//DATA VALUES AND SAVE THEM TO MEMORY
				default:
					//CREATE BUFFERS
					std::string Name = "";
					std::string Value = "";

					//FIND EQUAL SIGN TO PARSE VALUE
					CC = C;
					_Scan = 1;
					while (CC != '=')
					{
						CC = _Buffer[_Index + _Scan];
						_Scan++;
					}

					Name = _Buffer.substr(_Index, _Scan - 1);

					_Index = _Index + _Scan;
					_Scan = 1;
					while (CC != '\n' && _Index + _Scan < _Buffer.size())
					{
						CC = _Buffer[_Index + _Scan];
						_Scan++;
					}

					Value = _Buffer.substr(_Index, _Scan - 1);

					//WRITE VALUES TO MEMORY
					//SET INDEX TO END
					VALUES.emplace(Name, Value);
					_Index = _Index + _Scan;
					break;
			}
		}

		return S_OK;
	}
	else
	{
		//FILE DOES NOT EXIST OR
		//CANNOT BE ACCESSED
		_Input.close();
		return E_INI_FILE_BAD_PATH;
	}
}

HRESULT INIFile::GetValue(std::string Name, std::string& _Out)
{
	for (auto& V : VALUES)
	{
		if (V.first == Name)
		{
			_Out = V.second;

			return S_OK;
		}
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::GetValueByName(std::string Name, bool& _Out)
{
	std::string Buffer = "";
	if (GetValue(Name, Buffer) == S_OK)
	{
		std::transform(Buffer.begin(), Buffer.end(),
			Buffer.begin(), ::toupper);

		if (Buffer == "TRUE")
		{
			_Out = true;

			return S_OK;
		}
		else if (Buffer == "FALSE")
		{
			_Out = false;

			return S_OK;
		}

		return E_INI_DATA_UNEXPECTED_VALUE;
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::GetValueByName(std::string Name, int& _Out)
{
	std::string Buffer = "";
	if (GetValue(Name, Buffer) == S_OK)
	{
		_Out = std::stoi(Buffer);

		return S_OK;
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::GetValueByName(std::string Name, float& _Out)
{
	std::string Buffer = "";
	if (GetValue(Name, Buffer) == S_OK)
	{
		_Out = std::stof(Buffer);
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::GetValueByName(std::string Name, long& _Out)
{
	std::string Buffer = "";
	if (GetValue(Name, Buffer) == S_OK)
	{
		_Out = std::stoul(Buffer);
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::GetValueByName(std::string Name, DWORD& _Out)
{
	std::string Buffer = "";
	if (GetValue(Name, Buffer) == S_OK)
	{
		unsigned long DW = 0;
		std::istringstream SS(Buffer);
		SS >> std::hex >> DW;
		_Out = (DWORD)DW;

		return S_OK;
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::GetValueByName(std::string Name, const char*& _Out)
{
	std::string Buffer = "";
	if (GetValue(Name, Buffer) == S_OK)
	{
		_Out = Buffer.c_str();

		return S_OK;
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::GetValueByName(std::string Name, std::string& _Out)
{
	std::string Buffer = "";
	if (GetValue(Name, Buffer) == S_OK)
	{
		_Out = Buffer;

		return S_OK;
	}

	return E_INI_DATA_INVALID_NAME;
}

HRESULT INIFile::ToString(std::string& _Out)
{
	std::string Out = "";

	//TRY CONSTRUCTING OUTPUT BUFFER
	try
	{
		//APPEND ALL VALUES TO BUFFER
		for (auto& V : VALUES)
		{
			Out.append(V.first + "=" + V.second + "\n");
		}

		//INSERT HEADERS WHERE THEY BELONG
		for (auto& H : HEADERS)
		{
			Out.insert(H.second, H.first + '\n');
		}
	}
	//CATCH ANY ERRORS AND RETURN
	catch (...)
	{
		//COULD NOT READ VALUES FROM
		//MEMORY FOR ONE REASON OR ANOTHER
		return E_INI_DATA_INVALID_MEMORY;
	}

	//SUCCESS COPY TO OUTPUT
	_Out = Out;
	return S_OK;
}

HRESULT INIFile::Save()
{
	if (PATH != "")
	{
		return Save(PATH);
	}
	else
	{
		//NO FILE PATH PROVIDED
		//FILE WAS LIKELY CREATED IN MEMORY
		return E_INI_FILE_BAD_PATH;
	}
}

HRESULT INIFile::Save(std::string _Path)
{
	std::ofstream _Output(_Path.c_str(), std::ios::out);
	if (_Output.good())
	{
		std::string Buffer = "";
		ToString(Buffer);
		_Output << Buffer.c_str();
		_Output.close();

		//IF FILE WAS CREATED IN MEMORY
		//SAVE THIS PATH AS THE NEW FILE PATH
		if (PATH == "")
		{
			PATH = _Path;
		}

		return S_OK;
	}

	return E_ACCESSDENIED;
}

HRESULT INIFile::GetRelativeFilePath(std::string& _Path)
{
	_Path = PATH;

	return S_OK;
}

HRESULT INIFile::GetFullFilePath(std::string& _Path)
{
	char FullFile[MAX_PATH];

	if (GetFullPathNameA(PATH.c_str(), MAX_PATH, FullFile, nullptr) == S_OK)
	{
		_Path = std::string(FullFile);

		return S_OK;
	}
	else
	{
		_Path = std::string("");

		return E_ACCESSDENIED;
	}
}