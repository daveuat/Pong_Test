#include <genesis.h>
#include <resources.h>

// Global Variables
Sprite* ball;
Sprite* player1;
Sprite* player2;

// Paddle positions
int player1_pos_y = 64;
int player2_pos_y = 64;
int player_speed = 2;

// Ball position and speed
int ball_pos_x = 144;
int ball_pos_y = 112;
int ball_vel_x = 2;
int ball_vel_y = 2;

// Timer for speed increment
int timer = 0;
int speed_increment_interval = 300; // Increase speed every 300 frames (approximately every 5 seconds)

void moveBall() {
    ball_pos_x += ball_vel_x;
    ball_pos_y += ball_vel_y;

    // Increment the timer
    timer++;

    // Increase the ball's speed at regular intervals
    if (timer >= speed_increment_interval) {
        if (ball_vel_x > 0) {
            ball_vel_x++;
        } else {
            ball_vel_x--;
        }

        if (ball_vel_y > 0) {
            ball_vel_y++;
        } else {
            ball_vel_y--;
        }

        timer = 0; // Resets the timer
    }

    // Setup ball collision with top/bottom
    if(ball_pos_y <= 0 || ball_pos_y + 48 >= 224) ball_vel_y = -ball_vel_y;

    // Setup ball collision with Player 1's paddle (left side of screen)
    if(ball_pos_x <= 16 + 48 && ball_pos_y + 48 >= player1_pos_y && ball_pos_y <= player1_pos_y + 96) {
        ball_vel_x = -ball_vel_x;
        ball_pos_x = 16 + 48; // Ensure the ball doesn't get stuck in the paddle
    }

    // Setup ball collision with Player 2's paddle (right side of screen)
    if(ball_pos_x + 32 >= 288 && ball_pos_y + 48 >= player2_pos_y && ball_pos_y <= player2_pos_y + 96) {
        ball_vel_x = -ball_vel_x;
        ball_pos_x = 288 - 32; // Ensure the ball doesn't get stuck in the paddle
    }

    // Check to see if a player scored
    if(ball_pos_x <= 0 || ball_pos_x + 32 >= 320) {
        ball_pos_x = 144;
        ball_pos_y = 112;
        ball_vel_x = (ball_vel_x > 0) ? 2 : -2; // Reset to initial speed
        ball_vel_y = (ball_vel_y > 0) ? 2 : -2; // Reset to initial speed
        timer = 0; // Resets the timer
    }

    SPR_setPosition(ball, ball_pos_x, ball_pos_y);
}

void positionPlayers() {
    // Player 1 controls
    if(JOY_readJoypad(JOY_1) & BUTTON_UP) player1_pos_y -= player_speed;
    if(JOY_readJoypad(JOY_1) & BUTTON_DOWN) player1_pos_y += player_speed;

    // Player 2 controls (AI for now)
    if(ball_pos_y > player2_pos_y + 48) player2_pos_y += player_speed;
    else if(ball_pos_y < player2_pos_y + 48) player2_pos_y -= player_speed;

    SPR_setPosition(player1, 16, player1_pos_y);
    SPR_setPosition(player2, 288, player2_pos_y);
}

int main() {
    // Initialize the system hardware
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setBackgroundColor(0);

    // Initialize Sprite Engine
    SPR_init();

    // Load Sprites into the Sprite Engine
    player1 = SPR_addSprite(&paddle_image, 16, player1_pos_y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    player2 = SPR_addSprite(&paddle_image, 288, player2_pos_y, TILE_ATTR(PAL1, 0, FALSE, FALSE));
    ball = SPR_addSprite(&ball_image, ball_pos_x, ball_pos_y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    // Set palettes for the sprites
    PAL_setPalette(PAL1, paddle_image.palette->data, DMA);
    PAL_setPalette(PAL2, ball_image.palette->data, DMA);

    // Main Game Loop
    while(1) {
        moveBall();
        positionPlayers();

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}