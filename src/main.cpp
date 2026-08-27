

/*
TODO: Re-design the software so it doesn't load all images, instead it load ONLY the image it want to show <<
<< so it saves memory and stop the unwanted loading dely. Done!

- Fix the Arabic path that crash the program or it doesn't get supported!

*/

/*

To std::wstring (For Windows)

std::wstring toWide(const std::string& utf8) {
	int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	std::wstring wide(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wide.data(), size);
	wide.pop_back();
	return wide;
}

To std::string (For Cross‑Platform)

std::string toUTF8(const std::wstring& wide) {
	int size = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, utf8.data(), size, nullptr, nullptr);
	utf8.pop_back();
	return utf8;
}



*/

// Threading
#include <future>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace fs = std::filesystem;

fs::path imagesFolder;
sf::Vector2<int> canvace;
sf::Vector2u ScreenSize;
//std::vector<sf::Texture> texture;
sf::Texture texture;
sf::Sprite image(texture);

bool isPathsReady = false;
std::vector<fs::path> paths;

size_t index;

sf::Font font("font.otf");
sf::Text imageCount(font, "Copy path and open this window then press 'Enter'...", 35);
sf::Text imageRes(font, "", 25);

sf::Font ArFont("Marhey-VariableFont_wght.ttf");
sf::Text ArText(ArFont, "", 35);



template<typename T>
sf::Vector2<T> ScreenSizeNor(float x = 0.0f, float y = 0.0f) {
	float newVX = ScreenSize.x * x;
	float newVY = ScreenSize.y * y;
	return sf::Vector2<T>({ newVX, newVY });
}

void settingImage(sf::Sprite& img);

void setPaths(std::wstring pth = L"") {



	//std::cout << "Before Path is " << fs::absolute(pth).string() << "\n";

	if (pth.empty()) {
		std::cout << "To start browsing your images, please copy any folder's path and open this window and press 'Enter'\n";
		return;
	}

	
	
	paths.clear();

	imageCount.setString("Loading...");

	index = 0;

	try {
		for (const auto& path : fs::directory_iterator(fs::path(pth))) {
			auto Rpath = fs::path(path);
			std::string ext = Rpath.filename().extension().string();
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
				std::cout << Rpath.filename().string() << ", Loaded!\n";

				int w;
				int h;
				int c;

				if (stbi_info(fs::path(path).string().c_str(), &w, &h, &c)) {
					std::cout << w << "x" << h << std::endl;
					if (w > 10000 || h > 10000) {
						std::cout << "\"" << fs::path(path).filename().string() << "\" Did not load because it's too large!\n";
						continue;
					}
				}

				paths.push_back(Rpath);
			}
		}

	}
	catch (...) {
		std::cout << "No actual path detected!\n";
		return;
	}
	isPathsReady = true;
	std::cout << "Path is " << fs::absolute(pth).string() << "\n";



	settingImage(image);

}



void loadImage(size_t i, sf::Texture& txt) {
	if (paths.empty()) {
		throw "The 'images' directory is empty!\n";
		return;
	}

	

	int w;
	int h;
	int c;

	if (stbi_info(paths.at(i).string().c_str(), &w, &h, &c)) {
		if (w > 10000 || h > 10000) {
			throw "The image faild to load!. Larger than 10000x10000!\n";
			return;
		}
	}

	imageRes.setString(std::to_string(w) + "x" + std::to_string(h));
	imageRes.setOrigin({ imageRes.getLocalBounds().size.x, imageRes.getLocalBounds().size.y});
	imageRes.setPosition(ScreenSizeNor<float>(0.97f, 0.98f));
	std::cout << imageRes.getLocalBounds().size.x << std::endl;
	std::cout << imageRes.getLocalBounds().size.y << std::endl;

	std::cout << "Loading " << paths.at(i) << "\n";

	auto ext = paths.at(i).filename().extension();

	if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
		if (txt.loadFromFile(paths.at(i))) {



			return;
		}
		throw "No image detected at index " + std::to_string(i) + "!\n";
		return;
	}
	else {
		throw "Loading couldn't happen!\n";
		return;
	}






	return;

}

void settingImage(sf::Sprite& img) {

	try {

		loadImage(index, texture);


		texture.setSmooth(true);

		

		img.setTexture(texture, true);

		img.setPosition(ScreenSizeNor<float>(0.5f, 0.5f));
		img.setOrigin({ img.getLocalBounds().size.x / 2, img.getLocalBounds().size.y / 2 });


		sf::Vector2f newSize;

		newSize.x = static_cast<float>(ScreenSize.x - 50) / static_cast<float>(img.getTexture().getSize().x);
		newSize.y = static_cast<float>(ScreenSize.y - 50) / static_cast<float>(img.getTexture().getSize().y);

		float scale = std::min(newSize.x, newSize.y);

		//std::cout << scale << std::endl;

		img.setScale({ scale, scale });

		imageCount.setString(std::to_string(index + 1) + "/" + std::to_string(paths.size()));



	}
	catch (const char* e) {
		std::cout << e;

	}


}

//bool loadImages() {

	//if (!fs::is_empty(imagesFolder)) {
	//	for (const auto& file : std::filesystem::directory_iterator(std::filesystem::path("images"))) {
	//		auto ext = std::filesystem::path(file).filename().extension();
	//		if (ext == ".jpg" || ext == ".png") {
	//			//std::cout << "The file " << file << " is image!\n";
	//			std::cout << "The image is loaded!\n";
	//			texture.push_back(sf::Texture(file));

	//		}
	//		else {
	//			std::cout << "The file " << file << " is...\n";
	//			std::cout << ext << " is Unknown\n";
	//		}
	//	}

	//}
	//else {
	//	std::cout << "No images to show!\nPut some images into 'images' folder\n";
	//	return false;
	//}
	//std::cout << texture.size() << " Images loaded!\n";



//	return true;
//}



int main() {

	// System

	srand(0);

	ScreenSize = { 1080, 720 };

	sf::RenderWindow window(sf::VideoMode(ScreenSize), "Image viewer");

	imagesFolder = fs::path("images");
	
	canvace.x = ScreenSize.x;
	canvace.y = ScreenSize.y;

	imageCount.setOutlineColor(sf::Color(0, 0, 0, 125));
	imageCount.setOutlineThickness(0.75f);

	//imageRes.setString(std::to_string(w) + "x" + std::to_string(h));
	imageRes.setOrigin({ imageRes.getLocalBounds().size.x * 2, imageRes.getLocalBounds().size.y * 2});
	imageRes.setPosition(ScreenSizeNor<float>(1.0f, 1.0f));
	

	setPaths();

	

	//if (paths.empty()) {
	//	std::cout << "The 'images' directory is empty!\n";
	//	//return -1;
	//}

	//auto loadImgsT = nullptr;
	

	// Code

	index = 0;


	
	imageCount.setFillColor(sf::Color::White);

	imageCount.setPosition({ 0.0f, 0.0f });

	sf::Text txt(ArFont);

	txt.setString("TEXT");

	txt.setPosition(ScreenSizeNor<float>(0.0f, 0.5f));

	sf::RectangleShape textBG({35 * 2, 35});

	//textBG.setFillColor(sf::Color(0, 0, 0, 125));
	



	

	//if (!texture.loadFromFile("تنزيل.png")) {
	//	std::cout << "No images are located!\n";
	//	return -1;
	//}



	ArText.setPosition(ScreenSizeNor<float>(0.0f, 0.5f));
	

	bool imagesLoaded = false;

	while (window.isOpen()) {

		//if (!imagesLoaded && loadImgsT.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
		//	imagesLoaded = true;
		//	image.setTexture(texture.at(0), true);
		//	imageCount.setString(std::to_string(index + 1) + "/" + std::to_string(texture.size()));
		//}

		

		while (const auto event = window.pollEvent()) {


			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {

				
				if (key->scancode == sf::Keyboard::Scan::Right) {
					if (!isPathsReady)
						break;
					if (index < paths.size() - 1) {
						index++;
					}
					else {
						index = 0;
					}
					//std::cout << "Image index " << index << std::endl;
					settingImage(image);
				}
				if (key->scancode == sf::Keyboard::Scan::Left) {
					if (!isPathsReady)
						break;
					if (index > 0) {
						index--;
					}
					else {
						index = paths.size() - 1;
					}
					//std::cout << "Image index " << index << std::endl;
					settingImage(image);
				}
				if (key->scancode == sf::Keyboard::Scan::Up) {
					if (!isPathsReady)
						break;

					int oldIndex = index;
					while (index == oldIndex) {
						index = rand() % paths.size();
					}

					//std::cout << "Image index " << index << std::endl;
					settingImage(image);
				}
				

				/*auto loadImgsT = std::async(std::launch::async, loadImage);*/



				if (key->scancode == sf::Keyboard::Scan::Enter) {
					sf::String cb = sf::Clipboard::getString(); 

					//sf::U8String cbU8 = cb.toUtf8();
					auto cbU16 = cb.toUtf16();

					std::wstring word(reinterpret_cast<const wchar_t*>(cbU16.data()), cbU16.size());


					word.erase(std::remove(word.begin(), word.end(), '"'), word.end());

					
					std::wcout << word << std::endl;

					//std::string word = cb.toUtf8();
					//txt.setString(sf::String::fromUtf8(cb.begin(), cb.end()));
					//txt.setString(word);
					

					try {
						setPaths(word);
					}
					catch(const char* e){
						std::cout << e;
					}

				}
			}

			if (const auto& mm = event->getIf<sf::Event::MouseMoved>()) {
				//txt.setPosition(static_cast<sf::Vector2f>(mm->position));
				//imageCount.setPosition(static_cast<sf::Vector2f>(mm->position));
			}

		}

		//if (!imagesLoaded && loadImgsT.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
		//	imagesLoaded = true;
		//	image.setTexture(texture, true);
		//	imageCount.setString(std::to_string(index + 1) + "/" + std::to_string(paths.size()));
		//}
		

		window.clear(sf::Color::Black);


		//if (imagesLoaded) {
		//	window.draw(image);

		//}
			
		window.draw(image);
		window.draw(imageRes);
		window.draw(imageCount);
		
		window.draw(txt);


		
		window.display();
	

	}





	return 0;
}