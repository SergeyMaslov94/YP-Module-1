// Перегрузка унарного плюса и минуса
//----------------------------------------------------

// Здесь мы объявляем структуру, а не класс, так как поля структуры
// могут принимать произвольные значения
struct Vector2D {
    // Такая запись сообщает компилятору, чтобы он сгенерировал конструктор по умолчанию,
    // в котором поля x и y проинициализировал значениями, заданными при их объявлении
    Vector2D() = default;

    Vector2D(double x0, double y0)
            : x(x0), y(y0) {
    }

    // Задаем значения по умолчанию для полей структуры
    double x = 0.0;
    double y = 0.0;
};

// Операция умножения вектора на скаляр
Vector2D operator*(Vector2D vector, double scalar) {
    return {vector.x * scalar, vector.y * scalar};
}

// Операция умножения скаляра на вектор
Vector2D operator*(double scalar, Vector2D vector) {
    // Благодаря коммутативности операции умножения, мы можем выразить
    // умножение скаляра на вектор через операцию умножения вектора на скаляр
    return vector * scalar;
}

// Операция унарного плюса возвращает копию переданного вектора
Vector2D operator+(Vector2D v) {
    return v;
}

// Операция унарного минуса возвращает вектор с противоположным направлением
Vector2D operator-(Vector2D v) {
    return {-v.x, -v.y};
}

int top_2_theory_3_1()
{
    double scalar = 3;
    const Vector2D vector(4,5);

    auto mul1 = scalar * vector;
    auto mul2 = vector * scalar;

    const Vector2D v1{1,7};
    const Vector2D minus_v1 = - v1;
    const Vector2D plus_v1 = + v1;

    return 0;
}