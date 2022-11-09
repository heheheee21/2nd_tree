#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct knot//структура дерева
{
    string animal_guess = "";//если это конечный узел, то поле буде не пустое
    string question = "";
    knot* yesnode = nullptr;//указатели на ветки
    knot* nonode = nullptr;
};

void new_inf(string& question, string& animal)
{
    cout << "Какое животное вы хотели загадать? ";
    cin >> animal;

    cin.ignore();

    cout << "Какой вопрос можно было бы задать для этого животного? ";
    getline(cin, question);
}

void person_answer(string& answer)
{
    cin >> answer;
    while (answer != "да" && answer != "нет")
    {
        cout << "переделывай: ";
        cin >> answer;
    }
}

void new_branch(knot* current_branch, string& answer)
{
    string new_question, new_animal_guess;
    cout << "Вы загадали животное " << "- " << current_branch->animal_guess << "? ";
    person_answer(answer);
    if (answer == "да")
    {
        cout << "Комп угадал ура" << endl;
    }
    else
    {
        new_inf(new_question, new_animal_guess);
        knot* yesnode = new knot;
        knot* nonode = new knot;
        yesnode->animal_guess = new_animal_guess;
        nonode->animal_guess = current_branch->animal_guess;
        current_branch->question = new_question;
        current_branch->animal_guess = "";
        current_branch->yesnode = yesnode;
        current_branch->nonode = nonode;
    }
}

void game(knot* current_branch)
{
    string answer;
    if (current_branch->question != "")
    {
        cout << current_branch->question << endl;
        person_answer(answer);
        if (answer == "да")
            game(current_branch->yesnode);
        if (answer == "нет")
            game(current_branch->nonode);
    }
    else
        new_branch(current_branch, answer);
}

void fout_tree(knot* uzel)
{
    ofstream fout;
    fout.open("data.txt", ofstream::app);
    if (uzel == NULL)
        return;
    fout << uzel->question << endl;
    fout << uzel->animal_guess << endl;
    fout_tree(uzel->yesnode);
    fout_tree(uzel->nonode);
}

bool game_continue()
{
    string otvet;
    cout << "Хотите ли вы продолжить игру с новым деревом? ";
    cin >> otvet;
    if (otvet == "да")
        return 1;
    else
        return 0;
}

void fin_tree(knot*& uzel, ifstream& fin)
{
    string data_base;
    if (!fin.eof())
    {
        getline(fin, data_base);
        uzel->question = data_base;
        getline(fin, data_base);
        uzel->animal_guess = data_base;
    }
    if (!fin.eof() && uzel->animal_guess == "") 
    {
        knot* yesnode = new knot;
        uzel->yesnode = yesnode;
        fin_tree(yesnode, fin);
    }
    if (!fin.eof() && uzel->animal_guess == "") 
    {
        knot* nonode = new knot;
        uzel->nonode = nonode;
        fin_tree(nonode, fin);
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    knot* uzel = new knot;
    ifstream fin("data.txt");
    if (fin.peek() == EOF)
        uzel->animal_guess = "волчара";
    else
        fin_tree(uzel, fin);
    do
    {
        game(uzel);
    } 
    while (game_continue());
    ofstream fout("data.txt");
    remove("data.txt");
    fout_tree(uzel);
    return 0;
}