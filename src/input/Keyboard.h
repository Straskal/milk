#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

namespace milk {
	enum class Keys {
		ESCAPE = '\033',
		A = 'a',
		B = 'b',
		C = 'c',
		D = 'd',
		E = 'e',
		F = 'f',
		G = 'g',
		H = 'h',
		I = 'i',
		J = 'j',
		K = 'k',
		L = 'l',
		M = 'm',
		N = 'n',
		O = 'o',
		P = 'p',
		Q = 'q',
		R = 'r',
		S = 's',
		T = 't',
		U = 'u',
		V = 'v',
		W = 'w',
		X = 'x',
		Y = 'y',
		Z = 'z'
	};

	class Keyboard {
	public:
		virtual ~Keyboard() = default;

		virtual bool isKeyDown(Keys keycode) = 0;
		virtual bool isKeyPressed(Keys keycode) = 0;
		virtual bool isKeyReleased(Keys keycode) = 0;
	};
}

#endif