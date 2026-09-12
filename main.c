#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include "renderer.h"
#include "microui.h"

static mu_Context ctx;

static int text_width(mu_Font font, const char *str, int len) {
    if (len == -1) { len = strlen(str); }
    return r_get_text_width(str, len);
}

static int text_height(mu_Font font) {
    return r_get_text_height();
}

int main(int argc, char **argv) {
    /* 1. Inicializa o renderizador/janela SDL */
    r_init();

    /* 2. Inicializa o contexto do microui */
    mu_init(&ctx);
    ctx.text_width = text_width;
    ctx.text_height = text_height;

    //system("ifconfig > ifconfig.txt");
    char bin[300];

    FILE *fileIfConfig = fopen("ifconfig.txt", "r");
    char *texto = malloc(24*sizeof(char));
    fgets(bin, sizeof(bin), fileIfConfig);

    fread(texto,  24, 6, fileIfConfig);
    printf("\n%s\n", texto);

   // File *ifconfig = 

    /* 3. Loop principal */
    while (1) {
        /* Tratamento de eventos de entrada */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: return 0;
                case SDL_MOUSEMOTION: 
                    mu_input_mousemove(&ctx, e.motion.x, e.motion.y); break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP: {
                    int btn = (e.button.button == SDL_BUTTON_LEFT) ? MU_MOUSE_LEFT :
                              (e.button.button == SDL_BUTTON_RIGHT) ? MU_MOUSE_RIGHT : MU_MOUSE_MIDDLE;
                    if (e.type == SDL_MOUSEBUTTONDOWN) mu_input_mousedown(&ctx, e.button.x, e.button.y, btn);
                    else mu_input_mouseup(&ctx, e.button.x, e.button.y, btn);
                    break;
                }
            }
        }

        /* Declaração dos elementos visuais */
    mu_begin(&ctx);
        if (mu_begin_window(&ctx, "Simple Wireles ", mu_rect(10, 10, 200, 150))) {
        mu_layout_row(&ctx, 2, (int[]) { 60, -1 }, 0);

        mu_label(&ctx, "Your IP:");
        mu_label(&ctx, texto);

            

        //system("ifconfig > ifconfig.txt");



        mu_end_window(&ctx);
        }
        mu_end(&ctx);

        /* Renderização */
        r_clear(mu_color(80, 80, 80, 255));
        mu_Command *cmd = NULL;
        while (mu_next_command(&ctx, &cmd)) {
            switch (cmd->type) {
                case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
                case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
                case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
                case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
            }
        }
        r_present();
    }

    return 0;
}