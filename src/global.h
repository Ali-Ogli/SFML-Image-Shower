#pragma once

//================= [Headers] =================

#include <SFML/Graphics.hpp>	// For SFML window, render, shapes and more...
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>				// Printing to the consol
#include <fstream>				// Manipulate system files
#include <filesystem>			// Reading files from OS
#include <string>				// For std::string
#include <vector>				// For std::vector
#include <algorithm>			// For std::remove
#include <future>				// For std::future and std::async

#include "portable-file-dialogs.h"





namespace fs = std::filesystem;	// shorten std::filesystem to fs




inline sf::Clock DClock;									// Used to calculate delta time
inline sf::Vector2u ScreenSize{ 1080, 720 };							// Screen resolution
inline sf::Vector2<int> MaxImgSupport = { 15000, 15000 }; // The images resolution size support

// Window creating




inline std::vector<fs::path> paths;

inline fs::path path;
inline std::atomic<bool> isPathsReady = false;
inline std::atomic<bool> foundContent = false;

inline size_t index{ 0 };




// Graphical

inline sf::Image image;
inline sf::Texture texture;
inline sf::RectangleShape LoadingCube{ sf::Vector2f(25.0f, 100.0f) };
inline sf::Sprite sprite(texture);

// Text

inline sf::Font font;

inline sf::Text TcurrentPath(font, "", 15);
inline sf::Text imageCount(font, "", 35);
inline sf::Text imageRes(font, "No image loaded", 25);
inline std::vector<std::unique_ptr<sf::Text>> ImagesNames;


//================= [Templates] =================

inline tgui::Layout2d ScreenSizeNor(float x = 0.0f, float y = 0.0f) {
	float newVX = ScreenSize.x * x;
	float newVY = ScreenSize.y * y;
	return tgui::Layout2d(newVX, newVY);
}

template<typename T>
sf::Vector2<T> ScreenSizeNor(float x = 0.0f, float y = 0.0f) {
	float newVX = ScreenSize.x * x;
	float newVY = ScreenSize.y * y;
	return sf::Vector2<T>({ newVX, newVY });
}

template<typename T>
sf::Vector2<T> WindowSizeNor(sf::Window& window, float x = 0.0f, float y = 0.0f) {
	T newVX = window.getSize().x * x;
	T newVY = window.getSize().y * y;
	return sf::Vector2<T>({ newVX, newVY });
}

template<typename T>
sf::Vector2<T> WindowSizeNor(sf::Vector2u windowSize, float x = 0.0f, float y = 0.0f) {
	T newVX = windowSize.x * x;
	T newVY = windowSize.y * y;
	return sf::Vector2<T>({ newVX, newVY });
}

// Consol output

inline void Print(const std::string& text) {
	std::cout << std::endl;
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

// Lerp

inline float Lerp(float start, float end, float t) {
	return start + (end - start) * t;
}