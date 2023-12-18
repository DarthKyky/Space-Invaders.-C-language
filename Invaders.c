#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>

#define max_bullets 100000
#define EASY_ENEMIES 18
#define MEDIUM_ENEMIES 27
#define HARD_ENEMIES 36

void set_difficulty(int choice);
void check_collision();
void world_initialize();
void move_enemies();
void move_bullets();
void fire();
void enemy_on_screen();
void player_move();
void delay(unsigned int milliseconds);
void title();
void draw_enemy(int x, int y);
void draw_player(int x, int y);
void display_menu();

int max_enemies = EASY_ENEMIES;
int *enemy_alive;
int *enemy_x;
int *enemy_y;
int bullet_width = 1;
int bullet_height = 1;
int direction_type1 = 1;
int direction_type2 = 1;
int direction_type3 = 1;
int enemy_width[4]; // Масив для зберігання ширини ворогів
int enemy_height[3];
int bullet_x[max_bullets]; // Масив для зберігання позицій X снарядів
int bullet_y[max_bullets]; // Масив для зберігання позицій Y снарядів
int num_bullets = 0; // Поточна кількість снарядів
int player_x;
int player_y;

int main(){
    title();
    //draw_player(50, 50);
    display_menu();
    world_initialize();
    player_move();
    endwin();
    return 0;
}

void title(){
    initscr();
    noecho();
    mvprintw(3, 1, " ________  ________  ________  ________  _______           ___  ________   ___      ___ ________  ________  _______   ________  ________      ");
    mvprintw(4, 1, "|\\   ____\\|\\   __  \\|\\   __  \\|\\   ____\\|\\  ___ \\         |\\  \\|\\   ___  \\|\\  \\    /  /|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\|\\   ____\\     ");
    mvprintw(5, 1, "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\___|\\ \\   __/|        \\ \\  \\ \\  \\\\ \\  \\ \\  \\  /  / | \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\ \\  \\ \\  \\___|_    ");
    mvprintw(6, 1, " \\ \\_____  \\ \\   __  \\ \\   __  \\ \\  \\    \\ \\  \\_|/__       \\ \\  \\ \\  \\\\ \\  \\ \\  \\/  / / \\ \\   __  \\ \\  \\  \\ \\ \\  \\_|/_\\ \\   _  _\\ \\_____  \\   ");
    mvprintw(7, 1, "  \\|____|\\  \\ \\  \\_\\  \\ \\  \\ \\  \\ \\  \\____\\ \\  \\_|\\ \\       \\ \\  \\ \\  \\\\ \\  \\ \\    / /   \\ \\  \\ \\  \\ \\  \\__\\ \\ \\  \\_|\\ \\ \\  \\ \\  \\|____|\\  \\  ");
    mvprintw(8, 1, "    ____\\_\\  \\ \\__\\ \\__\\ \\__\\ \\__\\ \\_______\\ \\_______\\       \\ \\__\\ \\__\\\\ \\__\\ \\__/ /     \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ ___\\_\\  \\ ");
    mvprintw(9, 1, "   |\\_________\\|__|\\|__|\\|__|\\|__|\\|_______|\\|_______|        \\|__|\\|__| \\|__|\\|__|/       \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|\\_________\\");
    mvprintw(10, 1, "   \\|_________|                                                                                                                   \\|_________|");
    mvprintw(LINES-1, COLS/2 - 14, "Press any key to continue...");
    getch();
    erase();
    refresh();
    endwin();
}
void display_menu() {
    mvprintw(3, 1, " ________  ________  ________  ________  _______           ___  ________   ___      ___ ________  ________  _______   ________  ________      ");
    mvprintw(4, 1, "|\\   ____\\|\\   __  \\|\\   __  \\|\\   ____\\|\\  ___ \\         |\\  \\|\\   ___  \\|\\  \\    /  /|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\|\\   ____\\     ");
    mvprintw(5, 1, "\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\___|\\ \\   __/|        \\ \\  \\ \\  \\\\ \\  \\ \\  \\  /  / | \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\ \\  \\ \\  \\___|_    ");
    mvprintw(6, 1, " \\ \\_____  \\ \\   __  \\ \\   __  \\ \\  \\    \\ \\  \\_|/__       \\ \\  \\ \\  \\\\ \\  \\ \\  \\/  / / \\ \\   __  \\ \\  \\  \\ \\ \\  \\_|/_\\ \\   _  _\\ \\_____  \\   ");
    mvprintw(7, 1, "  \\|____|\\  \\ \\  \\_\\  \\ \\  \\ \\  \\ \\  \\____\\ \\  \\_|\\ \\       \\ \\  \\ \\  \\\\ \\  \\ \\    / /   \\ \\  \\ \\  \\ \\  \\__\\ \\ \\  \\_|\\ \\ \\  \\ \\  \\|____|\\  \\  ");
    mvprintw(8, 1, "    ____\\_\\  \\ \\__\\ \\__\\ \\__\\ \\__\\ \\_______\\ \\_______\\       \\ \\__\\ \\__\\\\ \\__\\ \\__/ /     \\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\ \\__\\ ___\\_\\  \\ ");
    mvprintw(9, 1, "   |\\_________\\|__|\\|__|\\|__|\\|__|\\|_______|\\|_______|        \\|__|\\|__| \\|__|\\|__|/       \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|\\_________\\");
    mvprintw(10, 1, "   \\|_________|                                                                                                                   \\|_________|");
    int height, width;
    getmaxyx(stdscr, height, width);

    int menu_height = 8;
    int menu_width = 40;
    int start_y = (height - menu_height) / 2;
    int start_x = (width - menu_width) / 2;

    WINDOW *menu_win = newwin(menu_height, menu_width, start_y, start_x);
    box(menu_win, 0, 0);

    keypad(menu_win, true);
    refresh();
    wrefresh(menu_win);

    int choice = 0;
    int highlight = 1;
    int c;

    while (1) {
        int x = start_x + 2;
        int y = start_y + 2;

        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);

        attron(COLOR_PAIR(2));
        mvprintw(y - 2, x, "Main Menu");
        mvprintw(y, x, "Choose difficulty:");
        attroff(COLOR_PAIR(2));

        if (highlight == 1) {
            attron(COLOR_PAIR(1));
            mvprintw(y + 2, x, "1. Im too young, to fight with aliens");
            attroff(COLOR_PAIR(1));
        } 
        else{
            mvprintw(y + 2, x, "1. Im too young, to fight with aliens");
        }

        if (highlight == 2) {
            attron(COLOR_PAIR(1));
            mvprintw(y + 3, x, "2. I will hurt them plenty");
            attroff(COLOR_PAIR(1));
        } 
        else{
            mvprintw(y + 3, x, "2. I will hurt them plenty");
        }

        if (highlight == 3) {
            attron(COLOR_PAIR(1));
            mvprintw(y + 4, x, "3. Ultra violence");
            attroff(COLOR_PAIR(1));
        } 
        else{
            mvprintw(y + 4, x, "3. Ultra violence");
        }

        refresh();

        c = wgetch(menu_win);

        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = 3;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == 3)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // ENTER key
                choice = highlight;
                break;
            default:
                break;
        }

        if (choice != 0) {
            set_difficulty(choice);
            refresh();
            break;
        }
    }
}

void set_difficulty(int choice) {
    switch (choice) {
        case 1:
            max_enemies = EASY_ENEMIES;
            break;
        case 2:
            max_enemies = MEDIUM_ENEMIES;
            break;
        case 3:
            max_enemies = HARD_ENEMIES;
            break;
        default:
            // Обробка неправильного вибору
            break;
    }
        free(enemy_alive);
    free(enemy_x);
    free(enemy_y);

    // Виділення пам'яті для нових розмірів масивів
    enemy_alive = malloc(max_enemies * sizeof(int));
    enemy_x = malloc(max_enemies * sizeof(int));
    enemy_y = malloc(max_enemies * sizeof(int));

}

void draw_player(int x, int y) {
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    attron(COLOR_PAIR(3));
    mvprintw(y, x, " /^\\");
    mvprintw(y + 1, x, "/_|_\\");
    attroff(COLOR_PAIR(3));
    //fflush(stdout);
}

void draw_enemy(int x, int y){
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    mvprintw(y, x, "/-\\");
    mvprintw(y + 1, x, "|0|");
    mvprintw(y + 2, x, "\\-/");
    attroff(COLOR_PAIR(4));
}

void draw_enemy_2(int x, int y){
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(5));
    mvprintw(y, x, "(_)");
    mvprintw(y + 1, x, "(=)");
    mvprintw(y + 2, x, "\\_/ ");
    attroff(COLOR_PAIR(5));
}

void draw_enemy_3(int x, int y){
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(6));
    mvprintw(y, x, "\\^/");
    mvprintw(y + 1, x, "}*{");
    mvprintw(y + 2, x, "{&}");
    attroff(COLOR_PAIR(6));
}

void player_move() {
    player_x = 71;
    player_y = 41;
    noecho();
    nodelay(stdscr, TRUE);
    int end;
    while (1) {
        int alive_enemies = 0;
        for (int i = 0; i < max_enemies; i++) {
            if (enemy_alive[i]) {
                alive_enemies++;
            }
        }   
        erase();
        move_bullets();
        //system("clear");
        int input = getch();
        draw_player(player_x, player_y);
        check_collision(bullet_x, bullet_y, num_bullets, enemy_x, enemy_y, max_enemies);
        move_enemies();
        refresh();
        // Зчитування введення користувача
        
        scanf("%d", &input); // Отримати символ від користувача
        if(input != ERR){
            // Логіка руху гравця
            if (input == 'a' || input == 'A') {
                player_x--; // Рух вліво
            } 
            else if(input == 'd' || input == 'D') {
                player_x++; // Рух вправо
            }
            else if (input == 'w' || input == 'W') {
                refresh();
                fire();
                move_bullets();
            }
        }
        for (int i = 0; i < max_enemies; i++) {
            if (enemy_y[i] == (player_y - 4)) {
                attron(A_BOLD);
                mvprintw(LINES / 2 - 3, COLS/2 - 3, " We lost...");
                mvprintw(LINES / 2 - 2, COLS/2 - 14, "Aliens have taken over our planet.");
                mvprintw(LINES / 2 - 1, COLS/2 - 6, "Press Q to exit.");
                attroff(A_BOLD);
                refresh();
                while ((end = getch()) != 'Q' && end != 'q') {
                }
                return; // Вихід з функції у випадку програшу
            }
        }
        if (alive_enemies <= 0) {
            bullet_y[0] = '\0';
            bullet_x[0] = '\0';
            attron(A_BOLD);
            mvprintw(LINES/2 - 2, COLS/2 - 8, "Congratulations, Hero!");
            mvprintw(LINES/2 - 1, COLS/2 - 16, "You defended our world from attackers.");
            mvprintw(LINES/2 , COLS/2 - 13, "The whole planet is proud of you!");
            mvprintw(LINES/2 + 1, COLS/2 - 24, "Press Q on the onboard computer to exit the shuttle.");
            attroff(A_BOLD);
            refresh();
            while ((end = getch()) != 'Q' && end != 'q') {
                // Wait for the user to press 'Q' or 'q' to exit
            }
            break;
        }
        move_bullets();
        delay(20);
        system("clear");
    }
}

void fire() {
    if (num_bullets < max_bullets) {
        // Додати новий снаряд до списку
        bullet_x[num_bullets] = player_x + 2; // Початкова позиція снаряду поруч з гравцем
        bullet_y[num_bullets] = player_y; // Змінено на -1, якщо снаряд має летіти вгору
        num_bullets++;
    }
}

void move_bullets() {
    for (int i = 0; i < num_bullets; i++) {
        // Рухати кожен снаряд вгору
        bullet_y[i]--; // Снаряди летять вгору, тому зменшуємо y
        refresh();
        // Оновлення екрану
        attron(COLOR_PAIR(1));
        mvprintw(bullet_y[i], bullet_x[i], "^");
        attroff(COLOR_PAIR(1));
        refresh(); // Оновити екран
    }
}

void delay(unsigned int milliseconds) {
    unsigned int ms = milliseconds * 1000; // Перетворення мілісекунд у мікросекунди
    clock_t start_time = clock();

    while (clock() < start_time + ms) {
        // Порожній цикл для затримки
    }
}

void move_enemies() {
    static int direction = 1;
    int wall_reached = 0;

    for (int i = 0; i < max_enemies; i++) {
        if (enemy_x[i] >= 138 || enemy_x[i] <= 0) {
            wall_reached = 1;
            break;
        }
    }

    if (wall_reached) {
        for (int i = 0; i < max_enemies; i++) {
            enemy_y[i] += 2;
        }
        direction *= -1;
    }

    int type_1_end = max_enemies / 3;
    int type_2_end = 2 * (max_enemies / 3);

    for (int i = 0; i < max_enemies; i++) {
        if (enemy_alive[i]) {
            if (i < type_1_end) {
                enemy_x[i] += direction_type1 * direction;
                draw_enemy(enemy_x[i], enemy_y[i]); // Малювання ворогів першого типу
            } 
            else if (i >= type_1_end && i < type_2_end) {
                enemy_x[i] += direction_type2 * direction;
                draw_enemy_2(enemy_x[i], enemy_y[i]); // Малювання ворогів другого типу
            } 
            else {
                enemy_x[i] += direction_type3 * direction;
                draw_enemy_3(enemy_x[i], enemy_y[i]); // Малювання ворогів третього типу
            }
        } 
        else {
            // Якщо ворог вже "мертвий", можна пропустити його
            continue;
        }
    }

    refresh();
}

void world_initialize() {

    initscr(); 
    int i = 0;
    int gap = 10; 
    
    int type_1_end = max_enemies / 3;
    int type_2_end = 2 * (max_enemies / 3);

    for (i = 0; i < max_enemies; i++) {
        if (i < type_1_end || (i >= type_1_end && i < type_2_end) || i >= type_2_end) {
            enemy_alive[i] = 1; // Вороги цих типів спочатку вважаються живими
        }
    }

    for (i = 0; i < type_1_end; i++) {
        enemy_x[i] = 10 + i * gap; // Початкова позиція X для ворогів першого типу
        enemy_y[i] = 5;           // Рядок 1 (перший тип)
    }

    for (i = type_1_end; i < type_2_end; i++) {
        enemy_x[i] = 10 + (i - type_1_end) * gap; // Початкова позиція X для ворогів другого типу
        enemy_y[i] = 10;                 // Рядок 2 (другий тип)
    }

    for (i = type_2_end; i < max_enemies; i++) {
        enemy_x[i] = 10 + (i - type_2_end) * gap; // Початкова позиція X для ворогів третього типу
        enemy_y[i] = 15;                 // Рядок 3 (третій тип)
    }
}

void check_collision(int bullet_x[], int bullet_y[], int num_bullets, int enemy_x[], int enemy_y[], int max_enemies) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    for (int i = 0; i < num_bullets; i++) {
        for (int j = 0; j < max_enemies; j++) {
            if (enemy_alive[j] && bullet_x[i] >= enemy_x[j] && bullet_x[i] <= enemy_x[j] + 2 &&
                bullet_y[i] >= enemy_y[j] && bullet_y[i] <= enemy_y[j] + 2) {
                enemy_alive[j] = 0; // Відмітити ворога як "мертвого"
                attron(COLOR_PAIR(1));
                mvprintw(enemy_y[j], enemy_x[j], "\\|/");
                attron(COLOR_PAIR(2));
                mvprintw(enemy_y[j] + 1, enemy_x[j], "=+=");
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(1));
                mvprintw(enemy_y[j] + 2, enemy_x[j], "/|\\");
                attroff(COLOR_PAIR(1));
                refresh();
                delay(100);
            }
        }
    }
}
