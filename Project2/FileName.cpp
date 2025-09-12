#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

// Callback för att spara svaret från curl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
	size_t totalSize = size * nmemb;
	s->append((char*)contents, totalSize);
	return totalSize;
}

double fetchExchangeRate() {
	CURL* curl = curl_easy_init();
	std::string readBuffer;
	double rate = 0.0;

	if (curl) {
		// Frankfurter API kräver ingen nyckel
		std::string url = "https://api.frankfurter.app/latest?from=SEK&to=EUR";
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (res == CURLE_OK) {
			auto j = json::parse(readBuffer);
			if (j.contains("rates") && j["rates"].contains("EUR")) {
				rate = j["rates"]["EUR"];
			}
			else {
				cout << "Fel vid hämtning av växelkurs: " << j.dump() << endl;
			}
		}
	}
	return rate;
}

int main()
{
	double kronor;
	cout << "hur myket sek har du ";
	cin >> kronor;
	double ex;
	double exchangeRate = fetchExchangeRate();

	int frog = kronor * exchangeRate * 1000 + 0.5;
	
	double euro = frog / 1000.0;
	cout << "du har ";
	cout << euro ;
	cout << " euro";
			

	
	

	return 0;
}