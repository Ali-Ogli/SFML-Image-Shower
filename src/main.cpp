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

/*
TODO:

- Fix software freezing when loading an sprite!

- Add UI to show user what to press to interact with the application.

*/

//================= [Headers] =================

#include <SFML/Graphics.hpp> // For SFML window, render, shapes and more...
#include <iostream>				// Printing to the consol
#include <fstream>				// Manipulate system files
#include <filesystem>			// Reading files from OS
#include <string>				// For std::string
#include <vector>				// For std::vector
#include <algorithm>			// For std::remove
#include <future>				// For std::future and std::async

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"			// For image reading and stbi_info


namespace fs = std::filesystem;	// shorten std::filesystem to fs

//================= [Variables] =================


//System

sf::Clock DClock;
sf::Vector2u ScreenSize;
sf::Vector2<int> MaxImgSupport = {15000, 15000};


fs::path imagesFolder;
sf::Vector2<int> canvace;
//std::vector<sf::Texture> texture;
sf::Image image;
sf::Texture texture;
sf::RectangleShape LoadingCube{ sf::Vector2f(25.0f, 100.0f)};
sf::Sprite sprite(texture);

std::atomic<bool> isPathsReady = false;
std::vector<fs::path> paths;

size_t index;

sf::Font font;
sf::Text imageCount(font, "Copy path and open this window then press 'Enter'...", 35);
sf::Text imageRes(font, "No sprite loaded", 25);


std::atomic isFinished = true;

// Threads

std::future<void> loadingPath;

enum class ProgramState {
	ReP,		// 	ReadingPath
	Se,			// 	SettingImage
	NP,			//	No path
	NI,			//	No Image

	Re			// 	Ready
};

ProgramState state = ProgramState::NP;

//================= [Other work] =================


//================= [Templates] =================

// Consol output

template<typename T>
sf::Vector2<T> ScreenSizeNor(float x = 0.0f, float y = 0.0f) {
	float newVX = ScreenSize.x * x;
	float newVY = ScreenSize.y * y;
	return sf::Vector2<T>({ newVX, newVY });
}

void Print(const std::string& text) {
	
}

template <typename T, typename... Args>
void Print(const std::string& function_name, T first, Args... rest) {
	std::cout << function_name << " : " << first;
	Print(function_name, rest...);
}

// File writing

template <typename T>
void LogToFile(T message) {
	std::ofstream file("Log.txt", std::ios::app);

	if (file) {
		file << message << " \n===============\n";

	}
}

//================= [Functions] =================


void setPaths(std::wstring pth = L"") {

	bool foundContent = false;

	if (pth.empty()) {
		//std::cout << "To start browsing your images, please copy any folder's path and open this window and press 'Enter'\n";
		Print("Getting path", "To start browsing your images, please copy any folder's path and open this window and press 'Enter'\n");
		state = ProgramState::NP;
		return;
	}

	state = ProgramState::ReP;
	
	paths.clear();

	

	index = 0;

	try {
		isPathsReady = false;
		for (const auto& path : fs::directory_iterator(fs::path(pth))) {
			auto Rpath = fs::path(path);
			std::string ext = Rpath.filename().extension().string();
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
				//std::cout << Rpath.filename().string() << ", Loaded!\n"; // Looking at what we are storing.
				//std::cout << Rpath.filename().string() << ", Loaded!\n"; // Looking at what we are storing.
				Print("Getting path", Rpath.filename().string() + " Loaded\n");

				int w;
				int h;
				int c;

				if (stbi_info(fs::path(path).string().c_str(), &w, &h, &c)) {
					//std::cout << w << "x" << h << std::endl; // Taking look at the resolution.
					if (w > MaxImgSupport.x || h > MaxImgSupport.y) {

						Print("\nGetting path", " ================\n\n");
						Print("Getting path", fs::path(path).filename().string() + " Did not load because it's too large!\n",
							fs::path(path).filename().string() + " Resolution is " + std::to_string(w) + "/" + std::to_string(h) + " \n",
							"Allowed resolution is " + std::to_string(MaxImgSupport.x) + "/" + std::to_string(MaxImgSupport.y));
						Print("\nGetting path", " ================\n\n");

						//std::cout << "\n\"" << fs::path(path).filename().string() << "\" Did not load because it's too large!\n";
						//std::cout << fs::path(path).filename().string() << " Resolution is " << w << 'x' << h << std::endl;
						//std::cout << "Allowed resolution is 10000x10000\n\n";
						continue;
					}
				}

				paths.push_back(Rpath);
			}
		}

	}
	catch (...) {
		//std::cout << "No actual path detected!\n";
		Print("Getting path", "No actual path detected!\n");
		isPathsReady = false;
		state = ProgramState::NP;

		imageCount.setString("No path detected!");
		return;
	}

	if (paths.empty()) {
		//std::cout << "No images found in '" << fs::absolute(fs::path(pth)) << "'\n";
		Print("Getting path", "No images found in '" + fs::absolute(fs::path(pth)).string() + "'\n");
		isPathsReady = false;
		state = ProgramState::NI;

		imageCount.setString("No images found!");
		return;
	}

	isPathsReady = true;

	state = ProgramState::Re;

	//std::cout << "Path is " << fs::absolute(pth).string() << "\n";
	

	//std::cout << "Reading path is finished" << "\n";
	Print("Getting path", "Reading path is finished\n");
}



void loadImage(size_t i) {
	if (paths.empty()) {
		throw "Load Image: The 'images' directory is empty!\n";
		return;
	}

	int w, h, c = 0;

	

	if (stbi_info(paths.at(i).string().c_str(), &w, &h, &c)) {
		if (w > MaxImgSupport.x || h > MaxImgSupport.y) {
			throw "Load Image: The sprite faild to load!. Larger than " + std::to_string(MaxImgSupport.x) + "/" + std::to_string(MaxImgSupport.y);
			return;
		}
	}

	sf::String sfString(std::to_string(w) + "x" + std::to_string(h));
	imageRes.setString(sfString);


	//sf::String sfString(std::to_string(152) + "x" + std::to_string(435));
	
	//imageRes.setString(std::to_string(w) + "x" + std::to_string(h));
	
	imageRes.setOrigin({ imageRes.getLocalBounds().size.x, imageRes.getLocalBounds().size.y});
	imageRes.setPosition(ScreenSizeNor<float>(0.97f, 0.98f));

	std::cout << "Load Image: Loading " << paths.at(i) << "\n";

	auto ext = paths.at(i).filename().extension();

	if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
		if (image.loadFromFile(paths.at(i))) {



			return;
		}
		throw "No sprite detected at index " + std::to_string(i) + "!\n";
		return;
	}
	else {
		throw "Loading couldn't happen!\n";
		return;
	}

	return;

}

void resetImage() {

	texture = sf::Texture();
	sprite.setTexture(texture);

}

void settingImage(sf::Sprite& spr) {

	state = ProgramState::Se;

	isFinished = false;

	imageCount.setString("Loading next sprite");

	try {

		imageCount.setString("Loading sprite");

		loadImage(index);

		if (!texture.loadFromImage(image)) {
			return;
			throw "Cannot load from image!\n";
		}

		texture.setSmooth(true);

		
		
		imageCount.setString("Setting the sprite");

		spr.setTexture(texture, true);

		spr.setPosition(ScreenSizeNor<float>(0.5f, 0.5f));
		spr.setOrigin({ spr.getLocalBounds().size.x / 2, spr.getLocalBounds().size.y / 2 });



		sf::Vector2f newSize;

		newSize.x = static_cast<float>(ScreenSize.x - 50) / static_cast<float>(spr.getTexture().getSize().x);
		newSize.y = static_cast<float>(ScreenSize.y - 50) / static_cast<float>(spr.getTexture().getSize().y);

		float scale = std::min(newSize.x, newSize.y);

		//std::cout << scale << std::endl;

		spr.setScale({ scale, scale });

	}
	catch (const char* e) {
		std::cout << e;
		imageCount.setString("No images found!");
		imageCount.setFillColor(sf::Color::Green);
		isFinished = true;
		state = ProgramState::NI;
		return;
	}


	state = ProgramState::Re;
	isFinished = true;
	imageCount.setString(std::to_string(index + 1) + "/" + std::to_string(paths.size()));

}




int main() {

	// System

	srand(0);

	ScreenSize = { 1080, 720 };

	sf::RenderWindow window(sf::VideoMode(ScreenSize), "Image viewer beta 0.0.1");

	imagesFolder = fs::path("images");
	
	canvace.x = ScreenSize.x;
	canvace.y = ScreenSize.y;

	imageCount.setOutlineColor(sf::Color(0, 0, 0, 125));
	imageCount.setOutlineThickness(0.75f);

	//imageRes.setString(std::to_string(w) + "x" + std::to_string(h));
	imageRes.setOrigin({ imageRes.getLocalBounds().size.x * 2, imageRes.getLocalBounds().size.y * 2});
	imageRes.setPosition(ScreenSizeNor<float>(1.0f, 1.0f));
	
	LoadingCube.setOrigin({ LoadingCube.getLocalBounds().size.x / 2, LoadingCube.getLocalBounds().size.y / 2 });
	LoadingCube.setPosition(ScreenSizeNor<float>(0.5f, 0.5f));
	
	
	setPaths();



	index = 0;


	
	imageCount.setFillColor(sf::Color::White);

	imageCount.setPosition({ 0.0f, 0.0f });

	for (auto file : fs::directory_iterator(fs::current_path())) {
		if (fs::path(file).extension() == ".otf" || fs::path(file).extension() == ".ttf") {
			//LogToFile(fs::path(file).filename().string() + " Found font.");
			
			Print("Font", fs::path(file).filename().string() + " is found\n");
			if (font.openFromFile(fs::path(file).filename())) {
				Print("Font", "Assigning " + fs::path(file).filename().string() + ".\n");
				break;
			}
			else {
				LogToFile("Error loading font!");
				return -1;
			}

		}
		else {
			//Print("Font", fs::path(file).filename().string() + '\n');
			continue;
		}
			
		
		LogToFile("No font was found!\n Supported fonts are otf or ttf English!");
		Print("Font", "Plasee check Log.txt for errors!\n");
		std::cin.get();
		return -1;
			
	}


	imageRes.setFont(font);
	imageCount.setFont(font);

	imageRes.setOrigin({ imageRes.getLocalBounds().size.x, imageRes.getLocalBounds().size.y });
	imageRes.setPosition(ScreenSizeNor<float>(0.97f, 0.98f));

	sf::RectangleShape textBG({35 * 2, 35});
	

	bool imagesLoaded = false;

	while (window.isOpen()) {

		if (loadingPath.valid()) {
			if (loadingPath.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
				loadingPath.get();
				if(isPathsReady)
					settingImage(sprite);
			}
		}
				


		

		while (const auto event = window.pollEvent()) {


			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
				if (state == ProgramState::ReP || state == ProgramState::Se) continue;
				
				else if (state == ProgramState::Re) {

					if (key->scancode == sf::Keyboard::Scan::Right) {
						if (!isPathsReady)
							break;
						if (index < paths.size() - 1) {
							index++;
						}
						else {
							index = 0;
						}
						settingImage(sprite);
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
						settingImage(sprite);
					}
					if (key->scancode == sf::Keyboard::Scan::Up) {
						if (!isPathsReady)
							break;

						int oldIndex = index;
						while (index == oldIndex) {
							index = rand() % paths.size();
						}
						//std::cout << "Image index " << index << std::endl;
						settingImage(sprite);
					}

					if (key->scancode == sf::Keyboard::Scan::Delete) {
						resetImage();
					}
				
				}

				if (key->scancode == sf::Keyboard::Scan::Enter) {
					sf::String cb = sf::Clipboard::getString(); 

					//sf::U8String cbU8 = cb.toUtf8();
					auto cbU16 = cb.toUtf16();

					std::wstring word(reinterpret_cast<const wchar_t*>(cbU16.data()), cbU16.size());


					word.erase(std::remove(word.begin(), word.end(), '"'), word.end());

					
					//std::wcout << word << std::endl;

					//std::string word = cb.toUtf8();
					//txt.setString(sf::String::fromUtf8(cb.begin(), cb.end()));
					//txt.setString(word);
					if (cb.isEmpty()) {
						Print("Clipboard", "Can't paste that here!\n");
						continue;
					}

					try {
						imageCount.setString("Reading the path");
						//setPaths(word);
						state = ProgramState::ReP;
						loadingPath = std::async(std::launch::async, setPaths, word);
					}
					catch (const sf::Exception& e) {
						std::cout << "Clickboard error : " << e.what();
					}
					catch(const char* e){
						std::cout << e;
					}

				}
				


				

			}

			if (const auto& mm = event->getIf<sf::Event::MouseMoved>()) {
			}

		}

		//if (!imagesLoaded && loadImgsT.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
		//	imagesLoaded = true;
		//	sprite.setTexture(texture, true);
		//	imageCount.setString(std::to_string(index + 1) + "/" + std::to_string(paths.size()));
		//}
		
		
		float delta = DClock.restart().asSeconds();

		window.clear(sf::Color::Black);

		switch (state)
		{
		case ProgramState::Re:
			window.draw(sprite);
			break;
		case ProgramState::ReP:
		case ProgramState::Se:
			LoadingCube.rotate(sf::Angle(sf::radians(8.0f)) * delta);
			window.draw(LoadingCube);
			
			break;
		default:
			break;
		}


			
		//window.draw(sprite);

		window.draw(imageRes);
		window.draw(imageCount);
		


		
		window.display();
	

	}


	std::cin.get();


	return 0;
}