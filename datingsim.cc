#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <optional>
#include <stdexcept>

using std::string;
using std::vector;
using std::cout;
using std::cin;

// ─── Enums ────────────────────────────────────────────────────────────────────

enum class gender { m, f, d };  // Typ Gender, welcher nur m, f und d hat

string gender_to_str(gender g) { 
    switch (g) {
        case gender::m: return "männlich";
        case gender::f: return "weiblich";
        case gender::d: return "divers";
    }
    return "?"; // returns ? if no gender is given
}

gender str_to_gender(const string& s) {
    if (s == "m") return gender::m;
    if (s == "f") return gender::f;
    if (s == "d") return gender::d;
    throw std::invalid_argument("Ungültiges Geschlecht: " + s); // throws exception if invalid gender
}

int zufall(int n) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(1, n);
    return dis(gen);
}

// ─── User ─────────────────────────────────────────────────────────────────────

class user {
    string name_;
    unsigned int age_;
    gender own_gender_;
    vector<gender> preference_;   // welche Geschlechter werden präferiert
    // vector<string> interests_;
    // vector<string> disgusts_;

public:
    // Konstruktor
    user(string name, unsigned int age, gender g, vector<gender> pref)
    : name_(std::move(name)), age_(age), own_gender_(g), preference_(std::move(pref)) {}

    // Getter-Methoden
    const string&         name()       const { return name_; }
    unsigned int          age()        const { return age_; }
    gender                own_gender() const { return own_gender_; }
    const vector<gender>& preference() const { return preference_; }

    // Gibt true zurück, wenn dieser User das gewählte Gender präferiert
    bool likes_gender(gender g) const {
        return std::find(preference_.begin(), preference_.end(), g) != preference_.end();
    }

    void print() const {
        cout << "  Name: " << name_ << "\n"
             << "  Alter: " << age_ << "\n"
             << "  Geschlecht: " << gender_to_str(own_gender_) << "\n"
             << "  Präferenz: ";
        for (auto& p : preference_) cout << gender_to_str(p) << " ";
        cout << "\n";
    }

    void edit() {
        int input = 0;
        bool schleife = true;

        while (schleife == true) {
        cout << "\n1) Namen ändern\n"
             << "2) Alter ändern\n"
             << "3) Geschlecht ändern\n"
             << "4) Präferenz ändern\n"
             << "0) Zurück\n> ";
        
            if (!(cin >> input)) {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            switch (input) {
                case 1: {
                    cout << "Geben Sie einen neuen Namen ein:\n> ";
                    cin >> name_;
                    cout << "Der neue Name wurde gespeichert!\n";
                    break;
                }
                case 2: {
                    cout << "Geben Sie das neue Alter ein:\n> ";
                    cin >> age_;
                    cout << "Das neue Alter wurde gespeichert!\n";
                    break;
                }
                case 3: {
                    string owng_str;
                    cout << "Geben Sie ein neues Geschlecht ein:\n> ";
                    while (true) {
                        cin >> owng_str;
                        try { own_gender_ = str_to_gender(owng_str); break; }
                        catch (...) { cout << "Bitte m, f oder d eingeben: "; }
                    }
                    cout << "Das neue Geschlecht wurde gespeichert!\n";
                    break;
                }
                case 4: {
                    vector<gender> pref;
                    cin.ignore();
                    string zeile;
                    std::getline(cin, zeile);
                    // parse space-separated tokens
                    size_t pos = 0;
                    while (pos < zeile.size()) {
                        size_t space = zeile.find(' ', pos); // Leerzeichen im String finden
                        if (space == string::npos) space = zeile.size();
                        string tok = zeile.substr(pos, space - pos);
                        if (!tok.empty()) {
                            try { pref.push_back(str_to_gender(tok)); }
                            catch (...) { cout << "Unbekannt ignoriert: " << tok << "\n"; }
                        }
                        pos = space + 1;
                    }
                    if (pref.empty()) {
                        cout << "(Keine Präferenz angegeben – alle Geschlechter werden akzeptiert)\n";
                        pref = { gender::m, gender::f, gender::d };
                    }
                    preference_ = pref;
                    break;
                }
                case 0: {
                    schleife = false;
                    break;
                }
            }
        }
    }
};

// ─── Tisch ────────────────────────────────────────────────────────────────────

class tisch {
    user a_;
    user b_;
public:
    tisch(user a, user b) : a_(std::move(a)), b_(std::move(b)) {}

    void print(int nr) const {
        cout << "┌─ Tisch " << nr << " ──────────────────────────";
        if (nr < 10) { cout << "─";}
        if (nr < 100) { cout << "─";}
        cout << "\n│ " << a_.name() << " (" << gender_to_str(a_.own_gender()) << ", " << a_.age() << ")\n";
        cout << "│   ↔\n";
        cout << "│ " << b_.name() << " (" << gender_to_str(b_.own_gender()) << ", " << b_.age() << ")\n";
        cout << "└──────────────────────────────────────\n";
    }
};

// ─── Saal ─────────────────────────────────────────────────────────────────────

class saal {
    vector<tisch> tische_;
    vector<user>  warteliste_;  // ungematchte User

public:
    saal() = default;

    void user_hinzufuegen(user u) {
        warteliste_.push_back(std::move(u));
    }

    // Matcht zwei User wenn beide das Geschlecht des anderen präferieren
    bool sind_kompatibel(const user& a, const user& b) const {
        return a.likes_gender(b.own_gender()) && b.likes_gender(a.own_gender());
    }

    // Greedy-Matching: erste kompatible Paarung wird genommen
    void tische_zuteilen() {
        tische_.clear();
        vector<bool> gematcht(warteliste_.size(), false);

        for (size_t i = 0; i < warteliste_.size(); ++i) {
            if (gematcht[i]) continue;
            for (size_t j = i + 1; j < warteliste_.size(); ++j) {
                if (gematcht[j]) continue;
                if (sind_kompatibel(warteliste_[i], warteliste_[j])) {
                    tische_.emplace_back(warteliste_[i], warteliste_[j]);
                    gematcht[i] = gematcht[j] = true;
                    break;
                }
            }
        }

        // Nicht gematchte User ausgeben
        cout << "\n";
        bool jemand_ungemacht = false;
        for (size_t i = 0; i < warteliste_.size(); ++i) {
            if (!gematcht[i]) {
                if (!jemand_ungemacht) {
                    cout << "⚠ Keine passende Paarung gefunden für:\n";
                    jemand_ungemacht = true;
                }
                cout << "  - " << warteliste_[i].name() << "\n";
            }
        }
    }

    void tische_ausgeben() const {
        if (tische_.empty()) {
            cout << "Keine Tische belegt.\n";
            return;
        }
        cout << "\n=== Tischzuteilung ===\n";
        for (size_t i = 0; i < tische_.size(); ++i) {
            tische_[i].print(static_cast<int>(i + 1));
        }
    }

    void randomize() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(warteliste_.begin(), warteliste_.end(), g);
    } // geplant, um die Vektorreihenfolge zufällig zu verändern, damit nicht immer das selbe Match kommt

    void timer(int s); // zeitlimit geplant

    vector<user>& warteliste() { return warteliste_; } // Getter-Methode
};

// ─── Interface ────────────────────────────────────────────────────────────────

void trennlinie() {
    cout << "──────────────────────────────────────\n";
}

user user_eingabe() {
    string name;
    unsigned int age;
    string g_str;
    string pref_str;
    vector<gender> pref;

    cout << "  Name: ";
    cin >> name;

    cout << "  Alter: ";
    while (!(cin >> age) || age == 0) {
        cout << "Bitte eine gültige Zahl eingeben: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    cout << "  Geschlecht (m/f/d): ";
    while (true) {
        cin >> g_str;
        try { str_to_gender(g_str); break; }
        catch (...) { cout << "Bitte m, f oder d eingeben: "; }
    }
    gender g = str_to_gender(g_str);

    cout << "  Präferenz (m/f/d, mehrere mit Leerzeichen, Ende mit Enter+Enter):\n> ";
    cin.ignore();
    string zeile;
    std::getline(cin, zeile);
    // parse space-separated tokens
    size_t pos = 0;
    while (pos < zeile.size()) {
        size_t space = zeile.find(' ', pos);
        if (space == string::npos) space = zeile.size();
        string tok = zeile.substr(pos, space - pos);
        if (!tok.empty()) {
            try { pref.push_back(str_to_gender(tok)); }
            catch (...) { cout << "Unbekannt ignoriert: " << tok << "\n"; }
        }
        pos = space + 1;
    }
    if (pref.empty()) {
        cout << "  (Keine Präferenz angegeben – alle Geschlechter werden akzeptiert)\n";
        pref = { gender::m, gender::f, gender::d };
    }

    return user(name, age, g, pref);
}

void interface() {
    saal s;
    int wahl = 0;

    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║      Speed-Dating Manager v0.2       ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    while (true) {
        cout << "\n1) User hinzufügen\n"
             << "2) Alle User anzeigen\n"
             << "3) User bearbeiten\n"
             << "4) Tische zuteilen & anzeigen\n"
             << "0) Beenden\n"
             << "> ";

        if (!(cin >> wahl)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        trennlinie();

        switch (wahl) {
            case 1: {
                cout << "Neuen User anlegen:\n";
                user u = user_eingabe();
                cout << "✓ User \"" << u.name() << "\" hinzugefügt.\n";
                s.user_hinzufuegen(std::move(u));
                break;
            }
            case 2: {
                const auto& wl = s.warteliste();
                if (wl.empty()) {
                    cout << "Keine User registriert.\n";
                } else {
                    cout << "Registrierte User (" << wl.size() << "):\n";
                    for (const auto& u : wl) {
                        u.print();
                        cout << "  ─\n";
                    }
                }
                break;
            }
            case 3: {
                auto& wl = s.warteliste();
                int i = 1;
                if (wl.empty()) {
                    cout << "Keine User registriert.\n";
                } else {
                    cout << "Welcher User soll bearbeitet werden?\n ";
                    for (const auto& u : wl) {
                        cout << i << ") " << u.name() << "\n ";
                        i++;
                    }
                    cout << "> ";
                    int n;
                    cin >> n;
                    cout << "Aktuelle Einstellungen für " << wl[n - 1].name() << ":\n";
                    wl[n - 1].print();
                    wl[n - 1].edit();
                }
                break;
            }
            case 4: {
                if (s.warteliste().empty()) {
                    cout << "Keine User vorhanden.\n";
                    break;
                }
                s.randomize();
                s.tische_zuteilen();
                s.tische_ausgeben();
                break;
            }
            case 0:
                cout << "Auf Wiedersehen!\n";
                return;
            default:
                cout << "Ungültige Eingabe.\n";
        }
    }
}

int main() {
    interface();
    return 0;
}
