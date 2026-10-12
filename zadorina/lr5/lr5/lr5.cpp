#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// ============================================================
//  1. ХРАНИЛИЩА ДАННЫХ
// ============================================================

struct ArrayStorage {
    static const int MAXN = 1000;
    double xa[MAXN], ya[MAXN];
    int n;
    ArrayStorage() : n(0) {}
    void Clear() { n = 0; }
    void Insert(double x, double y) {
        if (n >= MAXN) return;
        int pos = 0;
        while (pos < n && xa[pos] < x) pos++;
        for (int i = n; i > pos; --i) {
            xa[i] = xa[i - 1];
            ya[i] = ya[i - 1];
        }
        xa[pos] = x; ya[pos] = y;
        n++;
    }
    int Count() const { return n; }
    double GetX(int i) const { return xa[i]; }
    double GetY(int i) const { return ya[i]; }
};

struct VectorStorage {
    vector<pair<double, double>> data;
    void Clear() { data.clear(); }
    void Insert(double x, double y) {
        auto it = data.begin();
        while (it != data.end() && it->first < x) ++it;
        data.insert(it, { x, y });
    }
    int Count() const { return (int)data.size(); }
    double GetX(int i) const { return data[i].first; }
    double GetY(int i) const { return data[i].second; }
};

// ============================================================
//  2. БАЗОВЫЙ КЛАСС TDat
// ============================================================

template <class S>
class TDat {
protected:
    S storage;
    double xmin, xmax, ymin, ymax;
    bool autoRange;
    double kZens;
    void UpdateRange() {
        if (storage.Count() == 0) {
            xmin = xmax = ymin = ymax = 0;
            return;
        }
        ymin = ymax = storage.GetY(0);
        for (int i = 1; i < storage.Count(); i++) {
            double y = storage.GetY(i);
            if (y < ymin) ymin = y;
            if (y > ymax) ymax = y;
        }
        if (autoRange) {
            xmin = xmax = storage.GetX(0);
            for (int i = 1; i < storage.Count(); i++) {
                double x = storage.GetX(i);
                if (x < xmin) xmin = x;
                if (x > xmax) xmax = x;
            }
        }
    }
public:
    TDat() : xmin(0), xmax(0), ymin(0), ymax(0), autoRange(true), kZens(2.0) {}
    virtual ~TDat() {}
    virtual void AddPoint(double x, double y) {
        storage.Insert(x, y);
        UpdateRange();
    }
    void RemoveAll() { storage.Clear(); UpdateRange(); }
    int GetCount() const { return storage.Count(); }
    double GetX(int i) const { return storage.GetX(i); }
    double GetY(int i) const { return storage.GetY(i); }
    virtual double GetValue(double x) const {
        for (int i = 0; i < storage.Count(); i++)
            if (fabs(storage.GetX(i) - x) < 1e-12) return storage.GetY(i);
        return 0.0;
    }
    double GetXmin() const { return xmin; }
    double GetXmax() const { return xmax; }
    double GetYmin() const { return ymin; }
    double GetYmax() const { return ymax; }
    void SetRange(double a, double b) { xmin = a; xmax = b; autoRange = false; }
    void SetAutoRange(bool b) { autoRange = b; }
    bool IsAutoRange() const { return autoRange; }
    void SetZens(double k) { kZens = k; }
    double GetZens() const { return kZens; }
    virtual vector<pair<double, double>> GetFilteredPoints() const {
        vector<pair<double, double>> res;
        for (int i = 0; i < storage.Count(); i++)
            res.push_back({ storage.GetX(i), storage.GetY(i) });
        return res;
    }
};

// ============================================================
//  3. ЛИНЕЙНАЯ ИНТЕРПОЛЯЦИЯ
// ============================================================

template <class S>
class TDatLinInt : public TDat<S> {
public:
    double GetValue(double x) const override {
        auto pts = this->GetFilteredPoints();
        if (pts.empty()) return 0.0;
        if (pts.size() == 1)
            return (fabs(pts[0].first - x) < 1e-12) ? pts[0].second : 0.0;
        for (auto& p : pts)
            if (fabs(p.first - x) < 1e-12) return p.second;
        for (size_t i = 0; i + 1 < pts.size(); i++) {
            if (x >= pts[i].first && x <= pts[i + 1].first) {
                double t = (x - pts[i].first) / (pts[i + 1].first - pts[i].first);
                return pts[i].second + t * (pts[i + 1].second - pts[i].second);
            }
        }
        return 0.0;
    }
};

// ============================================================
//  4. СПЛАЙН-ИНТЕРПОЛЯЦИЯ
// ============================================================

inline bool Solve4x4(double A[4][4], double b[4], double res[4]) {
    for (int col = 0; col < 4; col++) {
        int piv = col;
        for (int r = col + 1; r < 4; r++)
            if (fabs(A[r][col]) > fabs(A[piv][col])) piv = r;
        if (fabs(A[piv][col]) < 1e-15) return false;
        if (piv != col) {
            for (int c = 0; c < 4; c++) swap(A[col][c], A[piv][c]);
            swap(b[col], b[piv]);
        }
        for (int r = col + 1; r < 4; r++) {
            double f = A[r][col] / A[col][col];
            for (int c = col; c < 4; c++) A[r][c] -= f * A[col][c];
            b[r] -= f * b[col];
        }
    }
    for (int i = 3; i >= 0; i--) {
        double s = b[i];
        for (int j = i + 1; j < 4; j++) s -= A[i][j] * res[j];
        res[i] = s / A[i][i];
    }
    return true;
}

template <class S>
class TDatSplInt : public TDat<S> {
public:
    double GetValue(double x) const override {
        auto pts = this->GetFilteredPoints();
        int n = (int)pts.size();
        if (n == 0) return 0.0;
        if (n == 1)
            return (fabs(pts[0].first - x) < 1e-12) ? pts[0].second : 0.0;
        for (auto& p : pts)
            if (fabs(p.first - x) < 1e-12) return p.second;
        int idx = -1;
        for (int i = 0; i + 1 < n; i++) {
            if (x >= pts[i].first && x <= pts[i + 1].first) { idx = i; break; }
        }
        if (idx < 0) return 0.0;
        if (n < 4) {
            double t = (x - pts[idx].first) / (pts[idx + 1].first - pts[idx].first);
            return pts[idx].second + t * (pts[idx + 1].second - pts[idx].second);
        }
        int i0 = idx - 1;
        int i3 = idx + 2;
        if (i0 < 0) { i0 = 0; i3 = 3; }
        if (i3 >= n) { i3 = n - 1; i0 = i3 - 3; }
        double A[4][4], b[4], res[4];
        for (int r = 0; r < 4; r++) {
            double xv = pts[i0 + r].first;
            double xp = 1.0;
            for (int c = 0; c < 4; c++) {
                A[r][c] = xp;
                xp *= xv;
            }
            b[r] = pts[i0 + r].second;
        }
        if (!Solve4x4(A, b, res)) {
            double t = (x - pts[idx].first) / (pts[idx + 1].first - pts[idx].first);
            return pts[idx].second + t * (pts[idx + 1].second - pts[idx].second);
        }
        double y = 0, xp = 1;
        for (int c = 0; c < 4; c++) { y += res[c] * xp; xp *= x; }
        return y;
    }
};

// ============================================================
//  5. ЦЕНЗУРИРОВАНИЕ ВЫБРОСОВ
// ============================================================

template <class Base>
class CensoringMixin : public Base {
public:
    vector<pair<double, double>> GetFilteredPoints() const override {
        vector<pair<double, double>> pts;
        for (int i = 0; i < this->storage.Count(); i++)
            pts.push_back({ this->storage.GetX(i), this->storage.GetY(i) });
        int n = (int)pts.size();
        if (n < 3) return pts;
        double ymin = pts[0].second, ymax = pts[0].second;
        for (auto& p : pts) {
            if (p.second < ymin) ymin = p.second;
            if (p.second > ymax) ymax = p.second;
        }
        double range = ymax - ymin;
        if (range < 1e-12) return pts;
        double k = this->kZens;
        vector<bool> isOutlier(n, false);
        for (int i = 1; i + 1 < n; i++) {
            double y_prev = pts[i - 1].second;
            double y_cur = pts[i].second;
            double y_next = pts[i + 1].second;
            if ((y_cur - y_next > range * k) && (y_cur - y_prev > range * k))
                isOutlier[i] = true;
            if ((y_next - y_cur > range * k) && (y_prev - y_cur > range * k))
                isOutlier[i] = true;
        }
        vector<pair<double, double>> result;
        for (int i = 0; i < n; i++)
            if (!isOutlier[i]) result.push_back(pts[i]);
        return result;
    }
};

template <class S> using TDatLinIntZ = CensoringMixin<TDatLinInt<S>>;
template <class S> using TDatSplIntZ = CensoringMixin<TDatSplInt<S>>;

// ============================================================
//  6. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ МЕНЮ
// ============================================================

void PrintStats(const string& name, double xmin, double xmax, double ymin, double ymax, int count) {
    cout << "  " << name << ": точек=" << count
        << ", x∈[" << xmin << ", " << xmax << "]"
        << ", y∈[" << ymin << ", " << ymax << "]" << endl;
}

void ClearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================
//  7. МЕНЮ ДЛЯ ИНТЕРФЕЙСА НА МАССИВАХ
// ============================================================

void RunArrayInterface() {
    TDat<ArrayStorage>          dat;
    TDatLinInt<ArrayStorage>    lin;
    TDatSplInt<ArrayStorage>    spl;
    TDatLinIntZ<ArrayStorage>   linZ;
    TDatSplIntZ<ArrayStorage>   splZ;

    int choice;
    do {
        cout << "\n===== ИНТЕРФЕЙС: МАССИВЫ =====" << endl;
        PrintStats("TDat", dat.GetXmin(), dat.GetXmax(), dat.GetYmin(), dat.GetYmax(), dat.GetCount());
        PrintStats("TDatLinInt", lin.GetXmin(), lin.GetXmax(), lin.GetYmin(), lin.GetYmax(), lin.GetCount());
        PrintStats("TDatSplInt", spl.GetXmin(), spl.GetXmax(), spl.GetYmin(), spl.GetYmax(), spl.GetCount());
        PrintStats("TDatLinIntZ", linZ.GetXmin(), linZ.GetXmax(), linZ.GetYmin(), linZ.GetYmax(), linZ.GetCount());
        PrintStats("TDatSplIntZ", splZ.GetXmin(), splZ.GetXmax(), splZ.GetYmin(), splZ.GetYmax(), splZ.GetCount());

        cout << "\n1 - добавить точку" << endl;
        cout << "2 - удалить все точки" << endl;
        cout << "3 - вычислить f(x)" << endl;
        cout << "0 - вернуться в главное меню" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        if (choice == 1) {
            double x, y;
            cout << "Введите x: "; cin >> x;
            cout << "Введите f(x): "; cin >> y;
            dat.AddPoint(x, y);
            lin.AddPoint(x, y);
            spl.AddPoint(x, y);
            linZ.AddPoint(x, y);
            splZ.AddPoint(x, y);
            cout << "Точка добавлена во все объекты." << endl;
        }
        else if (choice == 2) {
            dat.RemoveAll();
            lin.RemoveAll();
            spl.RemoveAll();
            linZ.RemoveAll();
            splZ.RemoveAll();
            cout << "Все точки удалены." << endl;
        }
        else if (choice == 3) {
            double x;
            cout << "Введите x: "; cin >> x;
            cout << "TDat      f(" << x << ") = " << dat.GetValue(x) << endl;
            cout << "TDatLinInt f(" << x << ") = " << lin.GetValue(x) << endl;
            cout << "TDatSplInt f(" << x << ") = " << spl.GetValue(x) << endl;
            cout << "TDatLinIntZ f(" << x << ") = " << linZ.GetValue(x) << endl;
            cout << "TDatSplIntZ f(" << x << ") = " << splZ.GetValue(x) << endl;
        }
        else if (choice == 0) {
            cout << "Возврат в главное меню." << endl;
        }
        else {
            cout << "Неверный выбор." << endl;
        }
        if (choice != 0) {
            cout << "Нажмите Enter для продолжения...";
            ClearInput();
            cin.get();
        }
    } while (choice != 0);
}

// ============================================================
//  8. МЕНЮ ДЛЯ ИНТЕРФЕЙСА НА ВЕКТОРАХ
// ============================================================

void RunVectorInterface() {
    TDat<VectorStorage>          dat;
    TDatLinInt<VectorStorage>    lin;
    TDatSplInt<VectorStorage>    spl;
    TDatLinIntZ<VectorStorage>   linZ;
    TDatSplIntZ<VectorStorage>   splZ;

    int choice;
    do {
        cout << "\n===== ИНТЕРФЕЙС: ВЕКТОРЫ =====" << endl;
        PrintStats("TDat", dat.GetXmin(), dat.GetXmax(), dat.GetYmin(), dat.GetYmax(), dat.GetCount());
        PrintStats("TDatLinInt", lin.GetXmin(), lin.GetXmax(), lin.GetYmin(), lin.GetYmax(), lin.GetCount());
        PrintStats("TDatSplInt", spl.GetXmin(), spl.GetXmax(), spl.GetYmin(), spl.GetYmax(), spl.GetCount());
        PrintStats("TDatLinIntZ", linZ.GetXmin(), linZ.GetXmax(), linZ.GetYmin(), linZ.GetYmax(), linZ.GetCount());
        PrintStats("TDatSplIntZ", splZ.GetXmin(), splZ.GetXmax(), splZ.GetYmin(), splZ.GetYmax(), splZ.GetCount());

        cout << "\n1 - добавить точку" << endl;
        cout << "2 - удалить все точки" << endl;
        cout << "3 - вычислить f(x)" << endl;
        cout << "0 - вернуться в главное меню" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        if (choice == 1) {
            double x, y;
            cout << "Введите x: "; cin >> x;
            cout << "Введите f(x): "; cin >> y;
            dat.AddPoint(x, y);
            lin.AddPoint(x, y);
            spl.AddPoint(x, y);
            linZ.AddPoint(x, y);
            splZ.AddPoint(x, y);
            cout << "Точка добавлена во все объекты." << endl;
        }
        else if (choice == 2) {
            dat.RemoveAll();
            lin.RemoveAll();
            spl.RemoveAll();
            linZ.RemoveAll();
            splZ.RemoveAll();
            cout << "Все точки удалены." << endl;
        }
        else if (choice == 3) {
            double x;
            cout << "Введите x: "; cin >> x;
            cout << "TDat      f(" << x << ") = " << dat.GetValue(x) << endl;
            cout << "TDatLinInt f(" << x << ") = " << lin.GetValue(x) << endl;
            cout << "TDatSplInt f(" << x << ") = " << spl.GetValue(x) << endl;
            cout << "TDatLinIntZ f(" << x << ") = " << linZ.GetValue(x) << endl;
            cout << "TDatSplIntZ f(" << x << ") = " << splZ.GetValue(x) << endl;
        }
        else if (choice == 0) {
            cout << "Возврат в главное меню." << endl;
        }
        else {
            cout << "Неверный выбор." << endl;
        }
        if (choice != 0) {
            cout << "Нажмите Enter для продолжения...";
            ClearInput();
            cin.get();
        }
    } while (choice != 0);
}

// ============================================================
//  9. ГЛАВНОЕ МЕНЮ
// ============================================================

int main() {
    setlocale(LC_ALL, "ru");
    srand((unsigned)time(nullptr));

    int choice;
    do {
        cout << "\n==============================================" << endl;
        cout << "  ЛАБОРАТОРНАЯ РАБОТА №5: ИЕРАРХИЯ КЛАССОВ" << endl;
        cout << "==============================================" << endl;
        cout << "Выберите интерфейс для тестирования:" << endl;
        cout << "1 - интерфейс на массивах" << endl;
        cout << "2 - интерфейс на векторах" << endl;
        cout << "0 - выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        if (choice == 1) {
            RunArrayInterface();
        }
        else if (choice == 2) {
            RunVectorInterface();
        }
        else if (choice == 0) {
            cout << "Выход из программы." << endl;
        }
        else {
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (choice != 0);

    return 0;
}