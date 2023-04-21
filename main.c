#include <allegro.h>

typedef struct personnage
{
    BITMAP* bas[3];
    BITMAP* gauche[3];
    BITMAP* droite[3];
    BITMAP* haut[3];
    int x, y;
    int direction;
    int frame;
    int en_deplacement;
}t_personnage;

void separer_bitmap_personnage(t_personnage* personnage, BITMAP* bitmap)
{
    int taille_w = 48;
    int taille_h = 48;

    for (int j = 0; j < 3; j++) {
        personnage->bas[j] = create_sub_bitmap(bitmap, j * taille_w, 0 * taille_h, taille_w, taille_h);
        personnage->gauche[j] = create_sub_bitmap(bitmap, j * taille_w, 1 * taille_h, taille_w, taille_h);
        personnage->droite[j] = create_sub_bitmap(bitmap, j * taille_w, 2 * taille_h, taille_w, taille_h);
        personnage->haut[j] = create_sub_bitmap(bitmap, j * taille_w, 3 * taille_h, taille_w, taille_h);
    }
}

void dessiner_personnage(t_personnage* personnage, BITMAP* buffer)
{
    BITMAP* frame;
    switch (personnage->direction) {
        case 0:
            frame = personnage->bas[personnage->frame];
            break;
        case 1:
            frame = personnage->gauche[personnage->frame];
            break;
        case 2:
            frame = personnage->droite[personnage->frame];
            break;
        case 3:
            frame = personnage->haut[personnage->frame];
            break;
    }
    masked_blit(frame, buffer, 0, 0, personnage->x, personnage->y, frame->w, frame->h);
}


void maj_personnage(t_personnage* personnage)
{
    int vitesse = 1;
    personnage->en_deplacement = 0;
    if (key[KEY_UP]) {
        personnage->y -= vitesse;
        personnage->direction = 3;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_DOWN]) {
        personnage->y += vitesse;
        personnage->direction = 0;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_LEFT]) {
        personnage->x -= vitesse;
        personnage->direction = 1;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_RIGHT]) {
        personnage->x += vitesse;
        personnage->direction = 2;
        personnage->en_deplacement = 1;
    }
}

int main() {
    allegro_init();
    install_keyboard();

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    BITMAP *background = load_bitmap("../background_3.bmp", NULL);
    if (!background) {
        allegro_message("Erreur lors du chargement de 'background.bmp'\n");
        return 1;
    }

    BITMAP *sprite = load_bitmap("../sprite.bmp", NULL);
    if (!sprite) {
        allegro_message("Erreur lors du chargement de 'sprite.bmp'\n");
        destroy_bitmap(background);
        destroy_bitmap(buffer);
        return 1;
    }

    t_personnage personnage;
    personnage.direction = 0;
    personnage.frame = 0;
    personnage.x = SCREEN_W/2;
    personnage.y = SCREEN_H/2;

    separer_bitmap_personnage(&personnage, sprite);

    int frame_counter = 0;

    while (!key[KEY_ESC]) {

        maj_personnage(&personnage);

        stretch_blit(background, buffer, 0, 0, background->w, background->h, 0, 0, buffer->w, buffer->h);

        dessiner_personnage(&personnage, buffer);

        blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);

        frame_counter++;
        if (frame_counter >= 10) {
            if (personnage.en_deplacement) {
                personnage.frame = (personnage.frame + 1) % 3;
            } else {
                personnage.frame = 0;
            }
            frame_counter = 0;
        }

        rest(10);
    }

    for (int i = 0; i < 3; i++) {
        destroy_bitmap(personnage.bas[i]);
        destroy_bitmap(personnage.gauche[i]);
        destroy_bitmap(personnage.droite[i]);
        destroy_bitmap(personnage.haut[i]);
    }
    destroy_bitmap(sprite);
    destroy_bitmap(background);
    destroy_bitmap(buffer);

    return 0;
}
END_OF_MAIN();
