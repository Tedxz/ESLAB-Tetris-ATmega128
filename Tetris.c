#include "tetris.h"

typedef struct
{
    int8_t x;
    int8_t y;
    int8_t index;
    uint16_t brick;
} brick_t;

#define BRICK_TYPE                  7
#define BRICK_NUM_OF_TYPE           4

#define BRICK_HEIGHT                4
#define BRICK_WIDTH                 4

#define MAP_WIDTH                   10
#define MAP_HEIGHT                  20

#define BRICK_START_X               ((MAP_WIDTH / 2) - (BRICK_WIDTH / 2))

#ifndef NULL
    #define NULL    ((void *)0)
#endif

#define     SET_BIT(dat, bit)      ((dat) |= (0x0001 << (bit)))
#define     CLR_BIT(dat, bit)      ((dat) &= ~(0x0001 << (bit)))
#define     GET_BIT(dat, bit)      (((dat) & (0x0001 << (bit))) >> (bit))

static void (*draw_box)(uint8_t x, uint8_t y, uint8_t color) = NULL;
static uint8_t (*get_random_num)(void) = NULL;
static void (*return_next_brick_info)(const void *info) = NULL;
static void (*return_remove_line_num)(uint8_t line) = NULL;

static bool is_game_over = false;


static const uint16_t preview_brick_table[BRICK_TYPE] =
{
    0x0360,     // S
    0x0C60,     // Z
    0x0446,     // L
    0x0226,     // J
    0x2222,     // I
    0x0660,     // O
    0x0E40,     // T
};


// 方块数据表
static const uint16_t brick_table[BRICK_TYPE][BRICK_NUM_OF_TYPE] =
{
    { 0x6C00, 0x4620, 0x06C0, 0x8C40 },     // S
    { 0xC600, 0x2640, 0x0C60, 0x4C80 },     // Z
    { 0x88C0, 0xE800, 0x6220, 0x02E0 },     // L
    { 0x2260, 0x08E0, 0xC880, 0xE200 },     // J
    { 0x4444, 0x0F00, 0x2222, 0x00F0 },     // I
    { 0xCC00, 0xCC00, 0xCC00, 0xCC00 },     // O
    { 0xE400, 0x2620, 0x04E0, 0x8C80 }      // T
};


// 旋转掩码表
static const uint16_t rotate_mask[BRICK_TYPE][BRICK_NUM_OF_TYPE] =
{
    { 0xEE20, 0x66E0, 0x8EE0, 0xECC0 },     // S
    { 0xE660, 0x2EE0, 0xEE80, 0xCCE0 },     // Z
    { 0xECC0, 0xEE20, 0x66E0, 0x8EE0 },     // L
    { 0x2EE0, 0xCCE0, 0xEE80, 0xE660 },     // J
    { 0x7FCC, 0xEF33, 0x33FE, 0xCCF0 },     // I
    { 0xCC00, 0xCC00, 0xCC00, 0xCC00 },     // O
    { 0xE620, 0x26E0, 0x8CE0, 0xEC80 }      // T
};

// 下一个方块的y坐标初始值
static const int8_t brick_start_y[BRICK_TYPE] =
{
//   S   Z   L   J   I   O   T
    -2, -2, -3, -3, -4, -2, -2
};

static int16_t map[MAP_HEIGHT];
static int16_t map_backup[MAP_HEIGHT];

static brick_t curr_brick;              // 当前方块
static brick_t next_brick;              // 下一个方块

static brick_t create_new_brick(void)
{
    brick_t brick;
    uint8_t bt = get_random_num() % BRICK_TYPE;

    brick.x = BRICK_START_X;
    brick.y = brick_start_y[bt];

    brick.index = bt << 4;
    brick.brick = brick_table[bt][0];

    return brick;
}


void tetris_sync(void)
{
    uint8_t x, y;

    for (y = 0; y < MAP_HEIGHT; y++)
    {
        if (map[y] != map_backup[y])
        {
            for (x = 0; x < MAP_WIDTH; x++)
            {
                if (GET_BIT(map[y], x) != GET_BIT(map_backup[y], x))
                    draw_box(x, y, (uint8_t)GET_BIT(map[y], x));
            }
        }
    }

    for (y = 0; y < MAP_HEIGHT; y++)
        map_backup[y] = map[y];

    return;
}



void tetris_sync_all(void)
{
    uint8_t x, y;

    for (y = 0; y < MAP_HEIGHT; y++)
    {
        for (x = 0; x < MAP_WIDTH; x++)
        {
            draw_box(x, y, (uint8_t)GET_BIT(map[y], x));
        }
    }

    return;
}



bool tetris_is_game_over(void)
{
    return is_game_over;
}

static void draw_brick(const brick_t brick)
{
    uint8_t box_x, box_y;

    for (box_y = 0; box_y < BRICK_HEIGHT; box_y++)
    {
        for (box_x = 0; box_x < BRICK_WIDTH; box_x++)
        {
            if (brick.y + box_y >= 0
                && GET_BIT(brick.brick, 15 - (box_y * BRICK_WIDTH + box_x)))
            {
                SET_BIT(map[box_y + brick.y], box_x + brick.x);
            }
        }
    }

    return;
}




static void clear_brick(const brick_t brick)
{
    uint8_t box_x, box_y;

    for (box_y = 0; box_y < BRICK_HEIGHT; box_y++)
    {
        for (box_x = 0; box_x < BRICK_WIDTH; box_x++)
        {
            if (brick.y + box_y >= 0
                && GET_BIT(brick.brick, 15 - (box_y * BRICK_WIDTH + box_x)))
            {
                CLR_BIT(map[box_y + brick.y], box_x + brick.x);
            }
        }
    }

    return;
}


static bool is_conflict(const brick_t dest)
{
    int8_t box_y, box_x;
    bool exp = true;

    for (box_y = 0; box_y < BRICK_HEIGHT; box_y++)
    {
        for (box_x = 0; box_x < BRICK_WIDTH; box_x++)
        {
            if ((GET_BIT(dest.brick, (15 - (box_y * BRICK_WIDTH + box_x)))))
            {
                if ((dest.y + box_y) < 0)
                {
                    exp = (((box_x + dest.x) > (MAP_WIDTH - 1))        // 右边界
                        || ((box_x + dest.x) < 0));                           // 左边界
                        // || ((box_y + dest.y) > (MAP_HEIGHT - 1)));  // 下边界
                }
                else
                {
                    exp = (((box_x + dest.x) > (MAP_WIDTH - 1))        // 右边界
                        || ((box_x + dest.x) < 0)                             // 左边界
                        || ((box_y + dest.y) > (MAP_HEIGHT - 1))    // 下边界
                        || (GET_BIT(map[box_y + dest.y], (box_x + dest.x))));// 地图内
                }
                if (exp)
                    return true;
            }
        }
    }

    return false;
}


void tetris_init(void (*draw_box_to_map)(uint8_t x, uint8_t y, uint8_t color),
                 uint8_t (*get_random)(void),
                 void (*next_brick_info)(const void *info),
                 void (*remove_line_num)(uint8_t line))
{
    uint8_t i;

    draw_box = draw_box_to_map;
    get_random_num = get_random;
    return_next_brick_info = next_brick_info;
    return_remove_line_num = remove_line_num;
    is_game_over = false;

    for (i = 0; i < MAP_HEIGHT; i++)
        map[i] = 0;

    curr_brick = create_new_brick();
    next_brick = create_new_brick();

    if (return_next_brick_info != NULL)
        return_next_brick_info(&preview_brick_table[next_brick.index >> 4]);

    draw_brick(curr_brick);
    tetris_sync_all();

    return;
}

static void line_clear_check(void)
{
    uint8_t row, l;

    l = 0;

    for (row = 0; row < MAP_HEIGHT; row++)
    {
        if (map[row] >= 0x3FF)
        {
            l++;

            uint8_t i;
            for (i = row; i > 0; i--)
            {
                map[i] = map[i - 1];
            }
            map[0] = 0;
        }
    }

    if (return_remove_line_num != NULL)
        return_remove_line_num(l);

    return;
}



bool tetris_move(dire_t direction)
{
    brick_t dest_brick = curr_brick;
    bool is_move = false;

    switch ((uint8_t)direction)
    {
        case dire_left:
            dest_brick.x--;
            break;
        case dire_right:
            dest_brick.x++;
            break;
        case dire_down:
            dest_brick.y++;
            break;
        case dire_rotate:
        {
            uint8_t i = dest_brick.index & 0x0F;
            i++;
            dest_brick.index = (dest_brick.index & 0xF0) | (i % 4);
            dest_brick.brick = rotate_mask[dest_brick.index >> 4][dest_brick.index & 0x0F];

            break;
        }
        default:
            break;
    }

    clear_brick(curr_brick);

    if (!is_conflict(dest_brick))
    {
        if (direction == dire_rotate)
        {
            dest_brick.brick = brick_table[dest_brick.index >> 4][dest_brick.index & 0x0F];
        }
        curr_brick = dest_brick;
        is_move = true;
    }
    else
    {
        if (direction == dire_down)
        {
            draw_brick(curr_brick);
            if (curr_brick.y + 1 <= 0)
            {
                is_game_over = true;
            }
            line_clear_check();
            curr_brick = next_brick;
            next_brick = create_new_brick();
            if (return_next_brick_info != NULL)
                return_next_brick_info(&preview_brick_table[next_brick.index >> 4]);
        }
        is_move = false;
    }

    draw_brick(curr_brick);

    return is_move;
}


