

#include "floatingWindow.h"

FloatingWindows::FloatingWindows() {
	windowSize = { 500, 500 };
	offset = {};

	MouseState = MOUSESTATE::Non;

	AlwaysOnTop = false;

	isMouseInWindow = false;

}

// init new Floating window

void FloatingWindows::init() {
	//return;
	initWindow();
}

// init winows

void FloatingWindows::initWindow() {
	//windows.push_back(std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize), "Floating window"
	//	, sf::Style::None, sf::State::Windowed));

	windows.push_back(std::make_unique<Window>(windowSize, paths.at(index)));

	setWindowZIndex();
}

void FloatingWindows::closeWindow(int id) {
	windows.erase(windows.begin() + id);
}

void FloatingWindows::closeAllWindows() {

	windows.clear();

}

// Draw

void FloatingWindows::draw() {
	if (windows.empty()) return;

	//size_t valueInd = ind - 1;


	for (size_t i = 0; i < windows.size(); i++) {

		windows.at(i)->draw();

	}

}

void FloatingWindows::setWindowZIndex() {
	for (size_t i = 0; i < windows.size(); i++) {
		if (AlwaysOnTop) {
			Print("Floating Window", "Always on top.");
			HWND hwnd = windows.at(i)->getWindow().getNativeHandle();
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			//windows.at(i)->
		}
		else {
			Print("Floating Window", "Not always on top.");
			HWND hwnd = windows.at(i)->getWindow().getNativeHandle();
			SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}
}

void FloatingWindows::setWindowTransp(int id, BYTE transparencyValue) {

	HWND hwnd = windows.at(id)->getWindow().getNativeHandle();

	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	if (SetLayeredWindowAttributes(hwnd, 0, transparencyValue, LWA_ALPHA)) {
		Print("WindowFloating", "The window " + std::to_string(id) + " is transperant.");
		return;
	}

	Print("WindowFloating", "The window " + std::to_string(id) + " did not be transperant.");

	return;
}

void FloatingWindows::eventHandler() {

	for (size_t i = 0; i < windows.size(); i++) {

		while (const auto event = windows.at(i)->getWindow().pollEvent()) {

			//================= [Window Events] =================

			if (windows.at(i)->getWindow().hasFocus()) {
				//Print("FloatingWIndow", "Window " + std::to_string(i) + " has focus");
			}

			if (event->is<sf::Event::Closed>()) {
				closeWindow(i);
				break;
			}


			if (const auto* resized = event->getIf<sf::Event::Resized>()) {
				windows.at(i)->resizeWindow(resized->size);

			}


			if (event->is<sf::Event::MouseEntered>()) {
				isMouseInWindow = true;
			}
			else if (event->is<sf::Event::MouseLeft>()) {
				if ((MouseState == MOUSESTATE::Non)) {
					if (windows.at(i)->draggingPlace.getFillColor().a > 0) {
						windows.at(i)->draggingPlace.setFillColor(sf::Color(255, 255, 255, 0));
					}

					if (windows.at(i)->WinResizePlace.getFillColor().a > 0) {
						windows.at(i)->WinResizePlace.setFillColor(sf::Color(255, 255, 255, 0));
					}
				}
				isMouseInWindow = false;
			}


			//================= [Input Events] =================


			//===Keyboard Buttons

			if (const auto* KP = event->getIf<sf::Event::KeyPressed>()) {
				if (KP->scancode == sf::Keyboard::Scan::H) {
					AlwaysOnTop = !AlwaysOnTop;
					setWindowZIndex();
				}

				if (KP->scancode == sf::Keyboard::Scan::Escape) {
					closeWindow(i);
					break;
				}

				if (KP->scancode == sf::Keyboard::Scan::Space) {
					if (windows.at(i)->getWindow().hasFocus()) {
						setWindowTransp(i, 175);
					}
				}

				if (KP->scancode == sf::Keyboard::Scan::R) {
					windows.at(i)->sprite.setRotation(sf::Angle(sf::degrees(0)));
				}

				if (KP->scancode == sf::Keyboard::Scan::S) {

					windows.at(i)->resizeSpr();

				}

			}

			//===Mouse Buttons===

			// Mouse wheel

			if (const auto* MW = event->getIf<sf::Event::MouseWheelScrolled>()) {
				if (MW->wheel == sf::Mouse::Wheel::Vertical && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LControl)) {
					if (MW->delta > 0) {
						windows.at(i)->win.setSize({ windows.at(i)->win.getSize().x, windows.at(i)->win.getSize().y + 20 });
					}
					else if (MW->delta < 0) {
						windows.at(i)->win.setSize({ windows.at(i)->win.getSize().x, windows.at(i)->win.getSize().y - 20 });
					}

				}
				else if (MW->wheel == sf::Mouse::Wheel::Vertical && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift)) {
					if (MW->delta > 0) {
						windows.at(i)->win.setSize({ windows.at(i)->win.getSize().x + 10, windows.at(i)->win.getSize().y });
					}
					else if (MW->delta < 0) {
						windows.at(i)->win.setSize({ windows.at(i)->win.getSize().x - 10, windows.at(i)->win.getSize().y });
					}

				}

				else if (MW->wheel == sf::Mouse::Wheel::Vertical) {
					if (MW->delta > 0) {
						windows.at(i)->sprite.scale({ 1.05f, 1.05f });
					}
					if (MW->delta < 0) {
						windows.at(i)->sprite.scale({ 0.95f, 0.95f });
					}
				}

			}

			// Mouse Button pressed

			if (const auto* MB = event->getIf<sf::Event::MouseButtonPressed>()) {

				if (MB->position.x > windows.at(i)->draggingPlace.getPosition().x - windows.at(i)->draggingPlace.getSize().x
					&& MB->position.y < windows.at(i)->draggingPlace.getPosition().y + windows.at(i)->draggingPlace.getSize().y) {
					if (MB->button == sf::Mouse::Button::Left) {
						MouseState = MOUSESTATE::WiD;
						//draggingPlace.setFillColor(sf::Color(255, 255, 255, 170));
					}
				}
				else if (MB->position.x > windows.at(i)->WinResizePlace.getPosition().x - windows.at(i)->WinResizePlace.getSize().x
					&& MB->position.y > windows.at(i)->WinResizePlace.getPosition().y - windows.at(i)->WinResizePlace.getSize().y) {
					if (MB->button == sf::Mouse::Button::Left) {
						MouseState = MOUSESTATE::WiRe;
						//WinResizePlace.setFillColor(sf::Color(255, 255, 255, 170));
					}
				}

				else {
					if (MB->button == sf::Mouse::Button::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift)) {
						MouseState = MOUSESTATE::WiD;
					}
					else if (MB->button == sf::Mouse::Button::Left) {
						MouseState = MOUSESTATE::ImD;
					}
					else if (MB->button == sf::Mouse::Button::Middle) {
						MouseState = MOUSESTATE::ImR;
					}

				}

				switch (MouseState)
				{
				case FloatingWindows::MOUSESTATE::WiD:
					offset = sf::Mouse::getPosition() - windows.at(i)->getWindow().getPosition();
					break;
				case FloatingWindows::MOUSESTATE::WiRe:
					currMouPos = MB->position;
					break;
				case FloatingWindows::MOUSESTATE::ImD:
					sprOffset = sf::Vector2f(sf::Mouse::getPosition()) - windows.at(i)->sprite.getPosition();
					break;
				case FloatingWindows::MOUSESTATE::ImR:
					currMouPos = MB->position;
					break;
				case FloatingWindows::MOUSESTATE::ImRe:
					break;
				case FloatingWindows::MOUSESTATE::Non:
					break;
				default:
					break;
				}

				//windows.at(i)->setPosition(sf::Mouse::getPosition());
			}

			// Mouse Button released

			else if (const auto* MB = event->getIf<sf::Event::MouseButtonReleased>()) {
				if (MouseState == MOUSESTATE::WiRe) {
					windows.at(i)->win.setSize(windows.at(i)->win.getSize() + newSize);
				}
				MouseState = MOUSESTATE::Non;
			}

			// Mouse Moved

			if (const auto* MP = event->getIf<sf::Event::MouseMoved>()) {
				if (MouseState == MOUSESTATE::Non) {
					if (MP->position.x > windows.at(i)->draggingPlace.getPosition().x - windows.at(i)->draggingPlace.getSize().x
						&& MP->position.y < windows.at(i)->draggingPlace.getPosition().y + windows.at(i)->draggingPlace.getSize().y) {
						windows.at(i)->draggingPlace.setFillColor(sf::Color(255, 255, 255, 75));
					}
					else {
						windows.at(i)->draggingPlace.setFillColor(sf::Color(255, 255, 255, 0));
					}

					if (MP->position.x > windows.at(i)->WinResizePlace.getPosition().x - windows.at(i)->WinResizePlace.getSize().x
						&& MP->position.y > windows.at(i)->WinResizePlace.getPosition().y - windows.at(i)->WinResizePlace.getSize().y) {
						windows.at(i)->WinResizePlace.setFillColor(sf::Color(255, 255, 255, 75));
					}
					else {
						windows.at(i)->WinResizePlace.setFillColor(sf::Color(255, 255, 255, 0));
					}
				}

				sf::Angle an;
				float deltaX;




				switch (MouseState)
				{
				case FloatingWindows::MOUSESTATE::WiD:
					windows.at(i)->getWindow().setPosition(sf::Mouse::getPosition() - offset);
					break;
				case FloatingWindows::MOUSESTATE::WiRe:
					//Print("WindowFloating", std::to_string(currMouPos.x - MP->position.x));
					Print("WindowFloating", std::to_string((MP->position.y - currMouPos.y)));
					newSize = sf::Vector2u(MP->position - currMouPos);
					break;
				case FloatingWindows::MOUSESTATE::ImD:
					windows.at(i)->sprite.setPosition(sf::Vector2f(sf::Mouse::getPosition()) - sprOffset);
					break;
				case FloatingWindows::MOUSESTATE::ImR:
					an = sf::degrees(currMouPos.x - MP->position.x);
					deltaX = currMouPos.x - MP->position.x;
					windows.at(i)->sprite.rotate(sf::Angle(-sf::degrees(deltaX)));
					currMouPos = MP->position;
					break;
				case FloatingWindows::MOUSESTATE::ImRe:
					break;
				case FloatingWindows::MOUSESTATE::Non:
					break;
				default:
					break;
				}

			}
			else {
				//currMouPos = sf::Mouse::getPosition();
			}


		}
	}

}

	//================= [End of Events] =================
//============================================================================

