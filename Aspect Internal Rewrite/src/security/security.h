#pragma once
#define WIN32_LEAN_AND_MEAN
#include "../commons.h"
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <intrin.h>      
#include <iphlpapi.h>
#include "xorstr.h"

#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

class security_t
{
private:
	inline void get_file(const char* dllName, const char* fileName, char* buffer, int bfSize) {
		if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
			char* pathEnd = strrchr(buffer, '\\');
			strcpy(pathEnd + 1, fileName);
		}
		else {
			*buffer = 0;
		}
	}
	inline std::string replace_all(std::string subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}
public:
	char* version = "2.0.1";
	bool authenticated = false;

public:
	inline std::string download_url(const char* URL)
	{
		std::string result{};
		HINTERNET handle = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
		HINTERNET url;
		if (handle)
		{
			url = InternetOpenUrlA(handle, URL, NULL, NULL, NULL, NULL);
			if (url)
			{
				char buffer[2000];
				DWORD bytesRead;
				do
				{
					InternetReadFile(url, buffer, 2000, &bytesRead);
					result.append(buffer, bytesRead);
					memset(buffer, 0, 2000);
				} while (bytesRead);
				InternetCloseHandle(handle);
				InternetCloseHandle(url);
				return replace_all(result, "|n", "\r\n");
			}
		}
		InternetCloseHandle(handle);
		return replace_all(result, "|n", "\r\n");
	}

	inline std::string encrypt(std::string str_in, std::string key, std::string iv)
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
	inline std::string decrypt(char* str_in, char* key, char* iv)
	{
		std::string str_out;
		CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption((byte*)key, strlen(key), (byte*)iv);
		CryptoPP::StringSource decryptor(str_in, true,
			new CryptoPP::Base64Decoder(
				new CryptoPP::StreamTransformationFilter(decryption,
					new CryptoPP::StringSink(str_out)
				)
			)
		);
		return str_out;
	}
	
	inline std::string random_string(size_t length)
	{
		static std::string charset = _xor_("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890").str();
		std::string result;
		result.resize(length);

		for (int i = 0; i < length; i++)
			result[i] = charset[rand() % charset.length()];

		return result;
	}
	inline char* random_char_arr(size_t length)
	{
		static std::string charset = _xor_("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890").str();
		std::string result;
		result.resize(length);

		for (int i = 0; i < length; i++)
			result[i] = charset[rand() % charset.length()];

		return (char*)result.c_str();
	}
	
	inline bool check_version()
	{
		return download_url("http://aspectnetwork.net/aspect/internal/version.php") == version;
	}
	inline bool authenticate(char* username, char* password)
	{
		std::string key = random_string(32);
		srand((unsigned)time(NULL));
		std::string key_2 = random_string(32);
		std::string iv = random_string(16);

		std::string enc_username = encrypt(username, key, iv);
		std::string enc_password = encrypt(password, key, iv);

		std::string enckey = encrypt(key, key_2, iv);

		std::string iv64;
		std::string key264;
		CryptoPP::StringSource dummy0001(iv, true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(iv64)));
		CryptoPP::StringSource dummy0002(key_2, true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(key264)));

		std::string poststring = _xor_("?1=").str() + enckey + _xor_("&2=").str() + enc_username + _xor_("&3=").str() + enc_password + _xor_("&4=").str() + iv64 + _xor_("&5=").str() + key264;
		std::string baseurl = _xor_("http://aspectnetwork.net/aspect/check.php");

		std::string response64 = download_url((baseurl + poststring).c_str());
		std::string combo = enckey + enc_username + enc_password;
		std::string response;
		std::string responseundone;
		std::string response64undone;

		CryptoPP::StringSource dummy0003(combo, true,
			new CryptoPP::Base64Encoder(new CryptoPP::StringSink(response)));
		CryptoPP::StringSource dummy0004(response, true,
			new CryptoPP::Base64Decoder(new CryptoPP::StringSink(responseundone)));
		CryptoPP::StringSource dummy0005(response64, true,
			new CryptoPP::Base64Decoder(new CryptoPP::StringSink(response64undone)));

		authenticated = response64undone == responseundone;
		return authenticated;
	}
};