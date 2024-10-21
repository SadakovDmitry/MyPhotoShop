#ifndef SCENE_GUARD
#define SCENE_GUARD

const int GRAD_COEF = 50;

class Color {
public:
    int R;
    int G;
    int B;
    int T;

    Color(int R_, int G_, int B_) : R(R_), G(G_), B(B_), T(255){};

    int mul_chanel(int col_1, int col_2){
        return static_cast<int>((col_1 / 255.0 * col_2 / 255.0) * 255.0);
    }

    Color operator*(Color &sec_col) {
        return Color(mul_chanel(R, sec_col.R), mul_chanel(G, sec_col.G), mul_chanel(B, sec_col.B));
    }
    Color operator*(double num) {
        return Color(mul_chanel(R, num), mul_chanel(G, num), mul_chanel(B, num));
    }
    Color operator+(int num) {
        return Color(R + num, G + num, B + num);
    }
    Color operator+(Color &sec_col) {
        return Color(R + sec_col.R, G + sec_col.G, B + sec_col.B);
    }

    ~Color() {}
};

#endif
