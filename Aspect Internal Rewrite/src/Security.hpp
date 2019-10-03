#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cassert>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <WinInet.h>
#include <intrin.h>       
#include <iphlpapi.h>
#include "xorstr.h"
#pragma comment(lib, "wininet.lib")

void get_file(const char* dllName, const char* fileName, char* buffer, int bfSize) {
	GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
	if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
		char* pathEnd = strrchr(buffer, '\\');
		strcpy(pathEnd + 1, fileName);
	}
	else {
		*buffer = 0;
	}
}

namespace Aspect
{
	namespace Whitelist
	{
		std::string version = "1.7.1";

		const char* getMachineName()
		{
			static char computerName[1024];
			DWORD size = 1024;
			GetComputerName(computerName, &size);
			return &(computerName[0]);
		}

		std::string get_hwid()
		{
			std::string ss = "Unable to get HWID";

			UCHAR sz_filesys[255], sz_volname_buff[255];
			DWORD dw_serial;
			DWORD dw_mfl;
			DWORD dw_sys_flags;
			int error = 0;

			bool success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)sz_volname_buff, 255, &dw_serial, &dw_mfl, &dw_sys_flags, (LPTSTR)sz_filesys, 255);

			if (!success)
			{
				ss = "Not enough permissions";
			}

			std::stringstream error_stream;
			error_stream << dw_serial;
			return std::string(error_stream.str());
		}

		std::string replaceAll(std::string subject, const std::string& search, const std::string& replace)
		{
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos)
			{
				subject.replace(pos, search.length(), replace);
				pos += replace.length();
			}
			return subject;
		}

		std::string DownloadURL(const char* URL)
		{
			HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
			HINTERNET urlFile;
			std::string rtn;
			if (interwebs)
			{
				urlFile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);
				if (urlFile)
				{
					char buffer[2000];
					DWORD bytesRead;
					do
					{
						InternetReadFile(urlFile, buffer, 2000, &bytesRead);
						rtn.append(buffer, bytesRead);
						memset(buffer, 0, 2000);
					} while (bytesRead);
					InternetCloseHandle(interwebs);
					InternetCloseHandle(urlFile);
					std::string p = replaceAll(rtn, "|n", "\r\n");
					return p;
				}
			}
			InternetCloseHandle(interwebs);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}

		std::string encrypt(const std::string& str_in, const std::string& key, const std::string& iv)
		{
			std::string str_out;
			CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
			CryptoPP::StringSource encryptor(str_in, true,
				new CryptoPP::StreamTransformationFilter(encryption,
					new CryptoPP::Base64Encoder(
						new CryptoPP::StringSink(str_out),
						false
					)
				)
			);
			return str_out;
		}


		std::string decrypt(const std::string& str_in, const std::string& key, const std::string& iv)
		{

			std::string str_out;
			CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

			CryptoPP::StringSource decryptor(str_in, true,
				new CryptoPP::Base64Decoder(
					new CryptoPP::StreamTransformationFilter(decryption,
						new CryptoPP::StringSink(str_out)
					)
				)
			);
			return str_out;
		}

		std::string random_string(size_t length)
		{
			static std::string charset = _xor_("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890").str();
			std::string result;
			result.resize(length);

			for (int i = 0; i < length; i++)
				result[i] = charset[rand() % charset.length()];

			return result;
		}

		std::string random_stringalt(size_t length)
		{
			static std::string charset = _xor_("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890").str();
			std::string result;
			result.resize(length);

			for (int i = 0; i < length; i++)
				result[i] = charset[rand() % charset.length()];

			return result;
		}

		bool CheckVersion()
		{
			if (DownloadURL("http://aspectnetwork.net/aspect/internal/version.php") != version)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		bool CheckRobloxVersion()
		{
			std::string version = "version-f7da4996a1354505";
			if (DownloadURL("http://setup.roblox.com/version") != version)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		void PrintChangelog()
		{
			std::cout << DownloadURL("http://aspectnetwork.net/aspect/internal/changelog.php") << std::endl;
		}

		bool CheckWhitelist(std::string username, std::string password)
		{
			std::string key = random_string(32);
			//std::cout << key << std::endl;
			srand((unsigned)time(NULL));
			std::string key2 = random_string(32);
			//std::cout << key2 << std::endl;
			std::string iv = random_string(16);
			//std::cout << iv << std::endl;
			std::string encusername = encrypt(username, key, iv);
			std::string encpassword = encrypt(password, key, iv);
			std::string enckey = encrypt(key, key2, iv);
			std::string iv64;
			std::string key264;
			CryptoPP::StringSource ss(iv, true,
				new CryptoPP::Base64Encoder(new CryptoPP::StringSink(iv64)));
			CryptoPP::StringSource sss(key2, true,
				new CryptoPP::Base64Encoder(new CryptoPP::StringSink(key264)));
			std::string poststring = _xor_("?1=").str() + enckey + _xor_("&2=").str() + encusername + _xor_("&3=").str() + encpassword + _xor_("&4=").str() + iv64 + _xor_("&5=").str() + key264;
			std::string baseurl = _xor_("http://aspectnetwork.net/aspect/check.php");
			std::string url = baseurl + poststring;
			//std::cout << url << std::endl;
			std::string response64 = DownloadURL(url.c_str());
			std::string combo = enckey + encusername + encpassword;
			std::string response;
			std::string responseundone;
			std::string response64undone;
			CryptoPP::StringSource ssss(combo, true,
				new CryptoPP::Base64Encoder(new CryptoPP::StringSink(response)));
			CryptoPP::StringSource sssss(response, true,
				new CryptoPP::Base64Decoder(new CryptoPP::StringSink(responseundone)));
			CryptoPP::StringSource ssssss(response64, true,
				new CryptoPP::Base64Decoder(new CryptoPP::StringSink(response64undone)));
			if (response64undone == responseundone)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void CheckKillSwitch()
		{
			while (true)
			{
				if (!CheckWhitelist("Trial", "Check"))
				{
					ExitProcess(0);
				}
				Sleep(60000);
			}
		}

		void Updater()
		{

		}

		HKEY OpenKey(HKEY hRootKey, LPCSTR strKey)
		{
			HKEY hKey;
			LONG nError = RegOpenKeyEx(hRootKey, strKey, NULL, KEY_ALL_ACCESS, &hKey);

			if (nError == ERROR_FILE_NOT_FOUND)
			{
				std::cout << "Creating registry key: " << strKey << std::endl;
				nError = RegCreateKeyEx(hRootKey, strKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
			}

			if (nError)
				std::cout << "Error: " << nError << " Could not find or create " << strKey << std::endl;

			return hKey;
		}

		void SetVal(HKEY hKey, LPCTSTR lpValue, DWORD data)
		{
			LONG nError = RegSetValueEx(hKey, lpValue, NULL, REG_DWORD, (LPBYTE)& data, sizeof(DWORD));

			if (nError)
				std::cout << "Error: " << nError << " Could not set registry value: " << (char*)lpValue << std::endl;
		}

		DWORD GetVal(HKEY hKey, LPCTSTR lpValue)
		{
			DWORD data;		DWORD size = sizeof(data);	DWORD type = REG_DWORD;
			LONG nError = RegQueryValueEx(hKey, lpValue, NULL, &type, (LPBYTE)& data, &size);

			if (nError == ERROR_FILE_NOT_FOUND)
				data = 0; // The value will be created and set to data next time SetVal() is called.
			else if (nError)
				std::cout << "Error: " << nError << " Could not get registry value " << (char*)lpValue << std::endl;

			return data;
		}

		bool AutoLogin()
		{
			//std::cout << "Auto Logging in!" << std::endl;
			char path[MAX_PATH];
			get_file("Aspect.dll", "", path, MAX_PATH);
			std::string file;
			file += path;
			file += "credentials.ini";
			std::ifstream cFile(file);
			if (!cFile.good())
			{
				return false;
			}
			if (cFile.is_open())
			{
				int linenumber = 1;
				std::string username;
				std::string password;
				std::string line;
				while (getline(cFile, line))
				{
					if (linenumber == 1)
					{
						username = line;
						linenumber++;
					}
					else if (linenumber == 2)
					{
						password = line;
						linenumber++;
					}
					else
					{
						break;
					}
				}
			}
			return true;
		}
	}
}