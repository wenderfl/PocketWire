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

typedef struct dados{
    int mtu;
    char ip[16];
    char netMask[16];
    char broadcast[16];
}Dados;

int main(int argc, char **argv) {
    /* 1. Inicializa o renderizador/janela SDL */
    r_init();

    /* 2. Inicializa o contexto do microui */
    mu_init(&ctx);
    ctx.text_width = text_width;
    ctx.text_height = text_height;

    Dados rede;

    //system("ifconfig > ifconfig.txt");
    char bin[300];

    FILE *fileIfConfig = fopen("ifconfig.txt", "r");
    char *texto = malloc(24*sizeof(char));
    
    //Linha 1 descartada
    fgets(bin, sizeof(bin), fileIfConfig); 

    //pegando mtu - LINHA 1
    sscanf(bin, "%*s %*s %*s %d", &rede.mtu); 
    printf("\n%d\n", rede.mtu);

    //linha 2 - ipnet
    fgets(bin, sizeof(bin),fileIfConfig);
    
    //pegando ip
    sscanf(bin,"%s%s", rede.ip, rede.ip);
    printf("\n%s\n", rede.ip);

    //pegando netmask
    sscanf(bin,"%*s %*s netmask %s", rede.netMask);
    printf("\n%s\n", rede.netMask);

    //pegando broadcast
    sscanf(bin,"%*s %*s %*s %*s broadcast %s", rede.broadcast);
    printf("\n%s\n", rede.broadcast);

   // mu_Rect tela = mu_rect(0, 0, 800, 600);
 //   int opcoes = MU_OPT_NOTITLE | MU_OPT_NORESIZE | MU_OPT_NOMOVE;

    int flagMtu=0;
    char msgMtu[30];

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
    mu_Rect tela = mu_rect(0, 0, 800, 600);
    int opcoes = MU_OPT_NOTITLE | MU_OPT_NORESIZE | MU_OPT_NOFRAME;

    if (mu_begin_window_ex(&ctx, "PocketWire", tela, opcoes)) {
     //   if (mu_begin_window(&ctx, "PocketWire", mu_rect(0, 0, 1280, 720))) {
        mu_layout_row(&ctx, 4, (int[]) { 60, 60, 60, -1}, 0);

        char c[10];
        sprintf(c, "%d", rede.mtu);

        //char char_mtu = (char)rede.mtu;
        mu_label(&ctx, "MTU:");

        mu_label(&ctx, c);

        if(mu_button_ex(&ctx, "VERIFICAR",0,0)){
            flagMtu=1;
        }

        if(flagMtu==1 && (rede.mtu==1500))
            sprintf(msgMtu, "MTU normal.");
        else if(rede.mtu!=1500)
            sprintf(msgMtu, "Seu MTU é anormal.");

        mu_label(&ctx, msgMtu);

 
        mu_layout_row(&ctx, 2, (int[]) { 80, -1 }, 0);
        mu_label(&ctx, "IP:");
        mu_label(&ctx, rede.ip);

        mu_label(&ctx, "NetMask:");
        mu_label(&ctx, rede.netMask);

        mu_label(&ctx, "Broadcast:");
        mu_label(&ctx, rede.broadcast);


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