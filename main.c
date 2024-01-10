#include "build/assets2banks.h"
#include <PSGlib.h>
#include <SMSlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BG_TILES_OFFSET 0
#define SPRITE_TILES_OFFSET 0x90

void main(void) {
  SMS_init();
  SMS_setSpritePaletteColor(0, RGB(0, 1, 0));

  SMS_loadPSGaidencompressedTiles((unsigned char *)bgtiles_psgcompr,
                                  BG_TILES_OFFSET);
  SMS_loadSTMcompressedTileMap(0, 0, (void *)bgtilemap_stmcompr);
  SMS_loadBGPalette((void *)bgpalette_bin);

  SMS_setSpriteMode(SPRITEMODE_NORMAL);
  SMS_useFirstHalfTilesforSprites(true);
  SMS_loadPSGaidencompressedTiles((unsigned char *)spritetiles_psgcompr,
                                  SPRITE_TILES_OFFSET);
  SMS_loadSpritePalette((void *)spritepalette_bin);

  PSGPlay(music_psg);

  SMS_displayOn();

  uint8_t x = 0;
  uint8_t y = 0;

  for (;;) {

    SMS_initSprites();
    SMS_addSprite(x, y, SPRITE_TILES_OFFSET + 0);
    SMS_addSprite(x + 8, y, SPRITE_TILES_OFFSET + 1);
    SMS_addSprite(x, y + 8, SPRITE_TILES_OFFSET + 2);
    SMS_addSprite(x + 8, y + 8, SPRITE_TILES_OFFSET + 3);
    SMS_finalizeSprites();

    SMS_waitForVBlank();

    SMS_copySpritestoSAT();
    PSGFrame();

    if (x == 255) {
      y++;
    }
    x++;
    if (y == 191) {
      y = 0;
    }
  }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0);
SMS_EMBED_SDSC_HEADER(1, 0, 2024, 1, 10, "fabiodl",
                      "Simple devkitSMS Hello World", "Test of devkitSMS");
