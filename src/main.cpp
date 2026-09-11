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

	-Re-design the UI and build animation function that rely on Lerp helper and make values change inside.

*/



#include "floatingWindow.h"
#include "global.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"			// For image reading and stbi_info

//================= [Variables] =================

//System

sf::RenderWindow window(sf::VideoMode(ScreenSize), "Image viewer beta 0.0.4", sf::Style::Close | sf::Style::Resize);


// GUI

tgui::Gui MainGUI(window);




// Threads

std::future<void> loadingPath;






// Enums

enum class ProgramState {
	ReP,		// 	ReadingPath
	Se,			// 	SettingImage
	NP,			//	No path
	NI,			//	No Image

	Rea			// 	Ready
};

ProgramState state = ProgramState::NP;

//================= [Other work] =================






//================= [Helper Functions] =================

std::wstring toWide(const std::string& utf8) {
	int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	std::wstring wide(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wide.data(), size);
	wide.pop_back();
	return wide;
}

//================= [Classes] =================




//================= [Functions] =================

// Search the path if it's valid or include content in it.

void setPaths(std::wstring pth = L"") {


	path = pth;

	Print("Exist", std::to_string(!fs::exists(fs::path(path))) + "\n");

	if (path.empty() || !fs::exists(fs::path(path))) {
		Print("Getting path", "To start browsing your images, please copy any folder's path and open this window and press 'Enter'\n");
		state = ProgramState::NP;
		isPathsReady = false;
		foundContent = false;
		return;
	}
	
	paths.clear();

	

	index = 0;



	// Loop through the path folder to check if it got any of png, jpg, jpeg files, otherwise it's empty directory and nothing
	//																	will be pushed to 'paths'.
	if (fs::exists(fs::path(path))) {
		state = ProgramState::ReP;
		isPathsReady = false;
		foundContent = false;
		for (const auto& path : fs::directory_iterator(fs::path(path))) {
			auto Rpath = fs::path(path);
			std::string ext = Rpath.filename().extension().string();
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
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
						continue;
					}
				}
				paths.push_back(Rpath);
				ImagesNames.push_back(std::make_unique<sf::Text>(font, Rpath.filename().string(), 15));
			}
		}

		if (paths.empty()) { // If nothing pushed to 'paths' then return back.
			//std::cout << "No images found in '" << fs::absolute(fs::path(pth)) << "'\n";
			Print("Getting path", "No images found in '" + fs::absolute(path).string() + "'\n");
			isPathsReady = true;
			foundContent = false;
			state = ProgramState::NI;
			path = fs::path(path);
			return;
		}

		isPathsReady = true;
		foundContent = true;

		Print("Getting path", "Reading path is finished\n");
		return;
	}
	else {
		Print("Getting path", "No actual path detected!\n");
		isPathsReady = false;
		foundContent = false;
		path.clear();
		Print("path", path);
		state = ProgramState::NP;
		return;
	}
}

// Loading images from the specific path index from 'paths' to sf::Texture.
// Side note: this function might split into two parts, one to load the image data using thread
//																		and two to load them to sf::Texture.

void loadImage(size_t i) {
	if (paths.empty()) {
		throw "Load Image: The 'images' directory is empty!\n";
		return;
	}

	int w, h, c = 0;

	// See if image is larger than it intended to be.

	if (stbi_info(paths.at(i).string().c_str(), &w, &h, &c)) {
		if (w > MaxImgSupport.x || h > MaxImgSupport.y) {
			throw "Load Image: The sprite faild to load!. Larger than " + std::to_string(MaxImgSupport.x) + "/" + std::to_string(MaxImgSupport.y);
			return;
		}
	}

	sf::String sfString(std::to_string(w) + "x" + std::to_string(h));

	imageRes.setString(sfString);
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

// Reset the sf::Sprite to empty sf::Texture.

void resetImage() {

	texture = sf::Texture();
	sprite.setTexture(texture);

}

// Setting the loaded data from sf::Texture to sf::Sprite to render it to user.

void settingImage() {

	state = ProgramState::Se;

	try {

		loadImage(index);

		if (!texture.loadFromImage(image)) {
			return;
			throw "Cannot load from image!\n";
		}

		texture.setSmooth(true);


		sprite.setTexture(texture, true);

		//sprite.setTextureRect(sf::IntRect({0, 0}, { 350, 350}));

		 //Don't let sf::Sprite to get off the screen, if it's bigger make it small, if it smaller fit it to screen.

		sf::Vector2f newSize;

		sprite.setOrigin({ sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2 });
		sprite.setPosition(ScreenSizeNor<float>(0.5f, 0.5f));

		newSize.x = static_cast<float>(ScreenSize.x - 150) / static_cast<float>(sprite.getTexture().getSize().x);
		newSize.y = static_cast<float>(ScreenSize.y - 250) / static_cast<float>(sprite.getTexture().getSize().y);

		float scale = std::min(newSize.x, newSize.y);

	
		sprite.setScale({ scale, scale });


	}
	catch (const char* e) {
		std::cout << e;
		imageCount.setFillColor(sf::Color::Green);
		state = ProgramState::NI;
		return;
	}

	imageCount.setString(std::to_string(index + 1) + "/" + std::to_string(paths.size()));

	state = ProgramState::Rea;
	
	MainGUI.get<tgui::BitmapButton>("BitBFloatingImage")->setEnabled(true);

}


//================= [main function] =================



void initiliseUI() {

	tgui::Theme::setDefault("assets/UI/Black.txt");

	///===Top Bar Panel

	tgui::Panel::Ptr topBarPanel = tgui::Panel::create({ "100%", "5%" });
	tgui::VerticalLayout::Ptr VerticalTopBarButtons = tgui::VerticalLayout::create({ "40%", "100%" });
	tgui::HorizontalLayout::Ptr HorizontalTopBarButtons = tgui::HorizontalLayout::create();
	tgui::BitmapButton::Ptr OpFoBiBut = tgui::BitmapButton::create("Open folder");
	tgui::BitmapButton::Ptr FlWiBiBut = tgui::BitmapButton::create("Floating image");
	tgui::BitmapButton::Ptr AboutBiBut = tgui::BitmapButton::create("About");

	topBarPanel->setOrigin({ 1.0f, 0.0f });
	topBarPanel->setPosition(ScreenSizeNor(1.0f, -0.05f));

	OpFoBiBut->setImage("assets/folder.png");
	OpFoBiBut->setImageScaling(1.0f);

	FlWiBiBut->setImage("assets/window_icon.png");
	FlWiBiBut->setImageScaling(1.0f);
	FlWiBiBut->setEnabled(false);

	AboutBiBut->setImage("assets/Exclamation.png");
	AboutBiBut->setImageScaling(1.0f);


	HorizontalTopBarButtons->addSpace(0.05f);
	HorizontalTopBarButtons->add(OpFoBiBut, 0.7f, "BitBOpenFolder");
	HorizontalTopBarButtons->addSpace(0.05f);
	HorizontalTopBarButtons->add(FlWiBiBut, 0.85f, "BitBFloatingImage");
	HorizontalTopBarButtons->addSpace(0.05f);
	HorizontalTopBarButtons->add(AboutBiBut, 0.43f, "BitBAbout");
	HorizontalTopBarButtons->addSpace(0.5f);
	

	VerticalTopBarButtons->add(HorizontalTopBarButtons);
	VerticalTopBarButtons->insertSpace(0, 0.2f);
	VerticalTopBarButtons->insertSpace(2, 0.2f);


	topBarPanel->add(VerticalTopBarButtons);

	MainGUI.add(topBarPanel, "TopBarPanel");


	///===Lower Bar panel

	tgui::Panel::Ptr lowerBarPanel = tgui::Panel::create({"100%", "10%"});
	tgui::HorizontalLayout::Ptr HoriLower = tgui::HorizontalLayout::create();
	tgui::Picture::Ptr ArrowLeftBitBut = tgui::Picture::create("assets/triangle-up.png");
	tgui::Picture::Ptr ArrowRightBitBut = tgui::Picture::create("assets/triangle-up.png");
	
	ArrowLeftBitBut->setOrigin(0.5f, 0.5f);
	ArrowLeftBitBut->setPosition({"47%", "50%"});
	ArrowLeftBitBut->setRotation(-90.0f, {0.5f, 0.5f});
	
	ArrowRightBitBut->setOrigin(0.5f, 0.5f);
	ArrowRightBitBut->setPosition({"53%", "50%"});
	ArrowRightBitBut->setRotation(90.0f, {0.5f, 0.5f});

	lowerBarPanel->setOrigin(0.0f, 1.0f);
	lowerBarPanel->setPosition(ScreenSizeNor(0.0f, 1.0f));

	//lowerBarPanel->add(ArrowBitBut);

	//HoriLower->addSpace(0.05f);
	//HoriLower->add(ArrowBitBut, 0.04, "ArrowBitBut");
	//HoriLower->addSpace(0.5f);
	//lowerBarPanel->add(HoriLower);
	lowerBarPanel->add(ArrowLeftBitBut, "ArrowLeftBitBut");
	lowerBarPanel->add(ArrowRightBitBut, "ArrowRightBitBut");
	
	MainGUI.add(lowerBarPanel, "LowerBarPanel");

}


int main() {

	//===System===

	FloatingWindows fw;

	srand(time(0));

	window.setVerticalSyncEnabled(true);


	//================= [GUI components] =================

	initiliseUI();

	//tgui::VerticalLayout::Ptr topBar;
	
	bool isTopBarActive = false;

	
	






	MainGUI.get<tgui::Picture>("ArrowRightBitBut")->onClick([]() {
		if (!isPathsReady)
			return;
		if (index < paths.size() - 1) {
			index++;
		}
		else {
			index = 0;
		}
		settingImage();
		});

	MainGUI.get<tgui::Picture>("ArrowLeftBitBut")->onClick([]() {
		if (!isPathsReady)
			return;
		if (index > 0) {
			index--;
		}
		else {
			index = paths.size() - 1;
		}
		settingImage();
		});

	
	MainGUI.get<tgui::Button>("BitBAbout")->onPress([](){
		auto msg = pfd::message("About",
			"This software is about viewing images and create floating images around the screen.\nThis app is under MIT License, meaning you are allowed to change the code however you want.\n\nFor more information, please visit:\nhttps://github.com/Ali-Ogli/SFML-Image-Shower", pfd::choice::ok);
		});
	
	MainGUI.get<tgui::Button>("BitBOpenFolder")->onPress([&]() {
		auto pth = pfd::select_folder("Select images folder", "");
		if (pth.result().empty()) return;
		loadingPath = std::async(std::launch::async, setPaths, toWide(pth.result()));
		});
	
	MainGUI.get<tgui::BitmapButton>("BitBFloatingImage")->onPress([&fw]() {
		fw.init();
		});

	//================= [Handle OS's specific events] =================


	pfd::settings::verbose(true);

	if (!pfd::settings::available) {
		Print("Dialog", "This system doesn't have dialog, sad.\n");
		return -1;
	}

	

	//================================================================



	// Reading the current folder to search for fonts to use, currently only support otf and ttf.

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

	// Setting up the text

	imageCount.setOutlineColor(sf::Color(0, 0, 0, 125));
	imageCount.setOutlineThickness(0.75f);
	imageCount.setFillColor(sf::Color::White);
	imageCount.setOrigin({ 0, imageCount.getLocalBounds().size.y});
	imageCount.setPosition(ScreenSizeNor<float>(0.01f, 0.99f));
	Print("Main ImageCount", std::to_string(imageCount.getLocalBounds().size.y));

	imageRes.setOrigin({ imageRes.getLocalBounds().size.x * 2, imageRes.getLocalBounds().size.y * 2});
	imageRes.setPosition(ScreenSizeNor<float>(1.0f, 1.0f));
	
	TcurrentPath.setFillColor(sf::Color(255, 255, 255, 255));
	TcurrentPath.setOutlineColor(sf::Color::Black);
	TcurrentPath.setOutlineThickness(0.5f);

	// Shapes

	LoadingCube.setOrigin({ LoadingCube.getLocalBounds().size.x / 2, LoadingCube.getLocalBounds().size.y / 2 });
	LoadingCube.setPosition(ScreenSizeNor<float>(0.5f, 0.5f));
	
	// Launching the first path setup which is greeting the user.

	setPaths();

	// Setting up the font. Unnecessary, but put it here to make sure.

	imageRes.setFont(font);
	imageCount.setFont(font);


	//================= [Game loop] =================

	while (window.isOpen()) {

		// This check if the thread loadingPath that hold 'setPaths' is done, 
		//										then will fire this only if it actually found content inside the folder.
		if (loadingPath.valid()) {
			if (loadingPath.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
				loadingPath.get();
				Print("Loading from path", "Finished\n");

				if (state == ProgramState::Rea) {
					Print("Main loadingPath", "FloatingImage enable");
					MainGUI.get<tgui::BitmapButton>("BitBFloatingImage")->setEnabled(true);
				} 
				else {
					Print("Main loadingPath", "FloatingImage disable");
					MainGUI.get<tgui::BitmapButton>("BitBFloatingImage")->setEnabled(false);
				}

				

				switch (state)
				{
				case ProgramState::ReP:
					imageCount.setString("Loading...");
					TcurrentPath.setString(path.string());
					TcurrentPath.setOrigin({ 0, TcurrentPath.getLocalBounds().size.y + 10 });
					TcurrentPath.setPosition(ScreenSizeNor<float>(0.005f, 1.01f));
					settingImage();
					break;
				case ProgramState::Se:
					imageCount.setString("Loading...");
					break;
				case ProgramState::NP:
					imageCount.setString("");
					//imageCount.setString("No path detected");
					TcurrentPath.setString("");
					TcurrentPath.setOrigin({ 0, TcurrentPath.getLocalBounds().size.y + 10 });
					TcurrentPath.setPosition(ScreenSizeNor<float>(0.005f, 1.0f));
					pfd::message("No images found", "No path detected!", pfd::choice::ok, pfd::icon::error);
					break;
				case ProgramState::NI:
					imageCount.setString("");
					//imageCount.setString("Can't find images in this directory");
					TcurrentPath.setString(path.string());
					TcurrentPath.setOrigin({ 0, TcurrentPath.getLocalBounds().size.y + 10 });
					TcurrentPath.setPosition(ScreenSizeNor<float>(0.005f, 1.0f));
					pfd::message("No images found", "There's No images found in\n" + path.string(), pfd::choice::ok, pfd::icon::warning);
					break;
				case ProgramState::Rea:
					imageCount.setString(std::to_string(index + 1) + "/" + std::to_string(paths.size()));
					break;
				default:
					break;
				}

				
			}
		}

		float MPY = sf::Mouse::getPosition(window).y;
		float MPX = sf::Mouse::getPosition(window).x;

		if (MPX > ScreenSizeNor<float>(0.0f, 0.0f).x &&
			MPX < ScreenSizeNor<float>(1.0f, 0.0f).x) {
			if (MPY < ScreenSizeNor<float>(0.0f, 0.095f).y &&
				MPY > ScreenSizeNor<float>(0.0f, 0.0f).y) {
				if (!MainGUI.get("TopBarPanel")->isAnimationPlaying()) {
					MainGUI.get("TopBarPanel")->moveWithAnimation(ScreenSizeNor(1.0f, 0.0f), 150);
				}
			}
			else {
				MainGUI.get("TopBarPanel")->moveWithAnimation(ScreenSizeNor(1.0f, -0.05f), 100);
			}
		}
		else {
			MainGUI.get("TopBarPanel")->moveWithAnimation(ScreenSizeNor(1.0f, -0.05f), 100);
		}



		//================= [Events] =================

		while (const auto event = window.pollEvent()) {

			MainGUI.handleEvent(*event);

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const auto* rs = event->getIf<sf::Event::Resized>()) {
				sf::View view;
				if (rs->size.x < 1080 || rs->size.y < 720) {
					window.setSize({ 1080, 720 });
					view.setViewport(sf::FloatRect({0.0f, 0.0f}, sf::Vector2f(sf::Vector2u(1080, 720))));
					Print("Main events", "Viewport so small!");
					window.setView(view);
					ScreenSize = window.getSize();
				}
				else {
					view.setViewport(sf::FloatRect({0.0f, 0.0f}, sf::Vector2f(rs->size)));
					Print("Main events", "Viewport resizing");
					window.setView(view);
					ScreenSize = window.getSize();  
				}

					
				imageCount.setPosition(ScreenSizeNor<float>(0.0f, 0.0f));
				//view.setCenter({ 0.0f, 0.0f });
				
			}

			if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {

				// This used to make user use arrows to navigate images, right, left and up for random image.

				if (state == ProgramState::ReP || state == ProgramState::Se) continue;

				if (state == ProgramState::Rea) {
					if (key->scancode == sf::Keyboard::Scan::Right) {
						if (!isPathsReady)
							break;
						if (index < paths.size() - 1) {
							index++;
						}
						else {
							index = 0;
						}
						settingImage();
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
						settingImage();
					}
					if (key->scancode == sf::Keyboard::Scan::Up) {
						if (!isPathsReady)
							break;

						int oldIndex = index;
						while (index == oldIndex) {
							index = rand() % paths.size();
						}
						settingImage();
					}

					if (key->scancode == sf::Keyboard::Scan::Delete) {
						resetImage();
					}
				}

				if (key->scancode == sf::Keyboard::Scan::Backspace) {
					if (isPathsReady) {
						path = path.parent_path();
						loadingPath = std::async(std::launch::async, setPaths, path);
						Print("BackSlash", "Going up a path\n");
					}
					else {
						Print("BackSlash", "No valid path ready\n");;
					}
				}

				if (key->scancode == sf::Keyboard::Scan::Enter) {
					sf::String cb = sf::Clipboard::getString();

					auto cbU16 = cb.toUtf16();

					std::wstring word(reinterpret_cast<const wchar_t*>(cbU16.data()), cbU16.size());


					/* This function make sure to strip the address from "" if it has any.Example From "C:\" to C:\  */
					word.erase(std::remove(word.begin(), word.end(), '"'), word.end());



					if (cb.isEmpty()) { // If clipboard is empty or copied actual file, then will be considered empty.
						Print("Clipboard", "Can't paste that here!\n");
						continue;
					}

					try {
						state = ProgramState::ReP;
						loadingPath = std::async(std::launch::async, setPaths, word);
					}
					catch (const sf::Exception& e) {
						std::cout << "Clickboard error : " << e.what();
					}
					catch (const char* e) {
						std::cout << e;
					}

				}

			
			}

			if (const auto* MP = event->getIf<sf::Event::MouseMoved>()) {

			}



		}
		
		
		
		fw.eventHandler();
		
		float delta = DClock.restart().asSeconds();

		//================= [Draw] =================

		window.clear(sf::Color::Black);
		


		// SFML Graphic Main

		switch (state)
		{
		case ProgramState::Rea:
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

		// GUI

		MainGUI.draw();

		// SFML Multiple Graphic


		fw.draw();

		

		//window.draw(imageRes);
		window.draw(imageCount);
		//window.draw(TcurrentPath);
	

		window.display(); // Always the last Func!


	}

	//std::cin.get();


	return 0;
}