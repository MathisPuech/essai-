#include <stdio.h>
#include <allegro.h>
typedef struct personnage
{
    int x, y;
    BITMAP* haut[9];
    BITMAP* bas[9];
    BITMAP* gauche[9];
    BITMAP* droite[9];
    int direction, en_deplacement;
    int frame;
    int nb_vie;
}t_personnage;
void initialisationAllegro()
{
    allegro_init();
    set_color_depth(desktop_color_depth());
    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,900,650,0,0))!=0)
    {     allegro_message("Pb de mode graphique") ;
        allegro_exit();
        exit(EXIT_FAILURE); }
}
int detection_changement(int choix)
{
    if (key[KEY_ENTER])
    {
        rest(500);
        if (choix == 1) return 0;
        else return choix+1;
    }
    return choix;
}
void creation_sprite_perso(t_personnage* personnage, BITMAP* bitmap, int taille_w, int taille_h)
{
    for (int i = 0; i < 9; ++i) {
        personnage->haut[i] = create_sub_bitmap(bitmap, i * taille_w, 518, taille_w, taille_h);
        personnage->bas[i] = create_sub_bitmap(bitmap, i * taille_w, 648, taille_w, taille_h);
        personnage->gauche[i] = create_sub_bitmap(bitmap, i * taille_w, 584, taille_w, taille_h);
        personnage->droite[i] = create_sub_bitmap(bitmap, i * taille_w, 711, taille_w, taille_h);
    }
}
void charger_vie(BITMAP* buffer,t_personnage* pers, BITMAP* coeur ){
    for (int i = 0; i < pers->nb_vie; ++i) {
        masked_blit(coeur, buffer, 0, 0, 0+i*coeur->w, 0, buffer->w, buffer->h);
    }
}
void affichage_perso(t_personnage* personnage, BITMAP* buffer, int taille_w, int taille_h)
{
    BITMAP* PERSO;
    switch (personnage->direction)
    {
        case 0:
            PERSO = personnage->haut[personnage->frame];
            break;
        case 1:
            PERSO = personnage->bas[personnage->frame];
            break;
        case 2:
            PERSO = personnage->gauche[personnage->frame];
            break;
        case 3:
            PERSO = personnage->droite[personnage->frame];
            break;
    }
//    masked_blit(PERSO, buffer,0,0,personnage->x, personnage->y, PERSO->w, PERSO->h);
    masked_stretch_blit(PERSO, buffer,0,0,taille_w,taille_h, personnage->x, personnage->y, SCREEN_W/2, SCREEN_H/2);
}
void maj_personnage(t_personnage* personnage)
{
    if(mouse_x >= (SCREEN_W/2)+160) personnage->direction = 3;
    else personnage->direction = 2;
    /*
    if(key[KEY_UP]) personnage->direction = 0;
    if(key[KEY_DOWN]) personnage->direction = 1;
    if(key[KEY_LEFT]) personnage->direction = 2;
    if(key[KEY_RIGHT]) personnage->direction = 3;
    */
}
int main() {
    ///         les installations          ///
    initialisationAllegro();
    install_keyboard();
    install_mouse();
    ///         BITMAP
    BITMAP* sprite_octave;
    BITMAP* sprite_marina;
    BITMAP* BG;
    BITMAP* image_select_player;
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    sprite_octave = load_bitmap("../sprite_octave.bmp", NULL);
    sprite_marina = load_bitmap("../sprite_marina.bmp", NULL);
    BG = load_bitmap("../background_choix_du_joueur.bmp", NULL);
    image_select_player = load_bitmap("../selection_joueur.bmp", NULL);
    ///         struc
    t_personnage personnage[4];
    for (int i = 0; i < 4; ++i) {
        personnage[i].x = (SCREEN_W / 2) - 50;
        personnage[i].y = 175;
        personnage[i].direction = 1;
        personnage[i].en_deplacement = 1;
        personnage[i].frame = 0;
    }
    ///         variable
    int frame_counteur = 0;
    int taille_h = 68;
    int taille_w = 64;
    int ecart = 50;
    int choix_perso = 0;
    ///         SPRITE
    creation_sprite_perso(&personnage[0], sprite_octave, taille_w, taille_h);
    creation_sprite_perso(&personnage[1], sprite_marina, taille_w, taille_h);
    while (!key[KEY_ESC])
    {
        blit(BG, buffer,0,0,0,0,BG->w, BG->h);
//        rectfill(buffer, ecart, ecart, SCREEN_W - ecart, SCREEN_H - ecart, makecol(155,41,3));
        choix_perso = detection_changement(choix_perso);
        masked_blit(image_select_player, buffer, 0,0, SCREEN_W/2, 100, image_select_player->w, image_select_player->h);
        maj_personnage(&personnage[choix_perso]);
        affichage_perso(&personnage[choix_perso],buffer, taille_w, taille_h);
        if (frame_counteur>=10)
        {
            if(personnage[choix_perso].en_deplacement == 1) personnage[choix_perso].frame = ((personnage[choix_perso].frame + 1) %9);
            else personnage[choix_perso].frame = 1;
            frame_counteur = 0;
        }


        rest(1.0);
    }
}