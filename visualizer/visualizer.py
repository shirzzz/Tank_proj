import pygame
import sys
import re

# ----- CONFIG -----
CELL_SIZE = 60
FPS = 1
WRAP_EDGES = False  # Set to True to enable edge wrapping

# ----- GLOBALS -----
GRID_WIDTH = None
GRID_HEIGHT = None

grid = []  # Map grid (characters)
wall_hits = {}  # (x, y) -> hit count for weakening walls

# ----- CLASSES -----
class Tank:
    def __init__(self, player_id, tank_index, x, y, images_dict):
        self.player_id = player_id
        self.tank_index = tank_index
        self.x = x
        self.y = y
        self.direction = 'LEFT' if player_id == 1 else 'RIGHT'
        self.alive = True
        self.images = images_dict

    def move(self, step):
        if not self.alive:
            return
        dx, dy = self._direction_to_delta()
        new_x = self.x + dx * step
        new_y = self.y + dy * step
        if WRAP_EDGES:
            new_x %= GRID_WIDTH
            new_y %= GRID_HEIGHT
        # within bounds
        if 0 <= new_x < GRID_WIDTH and 0 <= new_y < GRID_HEIGHT:
            cell = grid[new_y][new_x]
            # collide with wall?
            if cell == '#':
                return
            # move onto mine => death
            if cell == '@':
                self.x, self.y = new_x, new_y
                self.alive = False
                return
            self.x, self.y = new_x, new_y

    def move_forward(self):
        self.move(1)

    def move_backward(self):
        self.move(-1)

    def _direction_to_delta(self):
        return {
            'UP': (0, -1), 'DOWN': (0, 1),
            'LEFT': (-1, 0), 'RIGHT': (1, 0),
            'UP_RIGHT': (1, -1), 'UP_LEFT': (-1, -1),
            'DOWN_RIGHT': (1, 1), 'DOWN_LEFT': (-1, 1)
        }.get(self.direction, (0, 0))

    def rotate_left45(self):
        self._rotate(-1)

    def rotate_right45(self):
        self._rotate(1)

    def rotate_left90(self):
        self._rotate(-2)

    def rotate_right90(self):
        self._rotate(2)

    def _rotate(self, step):
        directions = ['UP', 'UP_RIGHT', 'RIGHT', 'DOWN_RIGHT',
                      'DOWN', 'DOWN_LEFT', 'LEFT', 'UP_LEFT']
        if self.direction in directions:
            idx = directions.index(self.direction)
            self.direction = directions[(idx + step) % len(directions)]

    def shoot(self):
        if not self.alive:
            return None
        dx, dy = self._direction_to_delta()
        sx = self.x + dx
        sy = self.y + dy
        if WRAP_EDGES:
            sx %= GRID_WIDTH
            sy %= GRID_HEIGHT
        return Shell(sx, sy, dx, dy)

    def get_sprite(self):
        key = f"player{self.player_id}_{self.direction.lower().replace('_', '')}"
        return self.images.get(key)

class Shell:
    def __init__(self, x, y, dx, dy):
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.radius = CELL_SIZE // 6
        self.alive = True

    def update(self, tanks):
        # move two steps, checking collisions cell by cell
        for _ in range(2):
            self.x += self.dx
            self.y += self.dy
            if WRAP_EDGES:
                self.x %= GRID_WIDTH
                self.y %= GRID_HEIGHT
            # bounds
            if not (0 <= self.x < GRID_WIDTH and 0 <= self.y < GRID_HEIGHT):
                self.alive = False
                return
            cell = grid[int(self.y)][int(self.x)]
            # hit wall
            if cell == '#':
                pos = (int(self.x), int(self.y))
                wall_hits[pos] = wall_hits.get(pos, 0) + 1
                if wall_hits[pos] >= 2:
                    grid[int(self.y)][int(self.x)] = '.'
                self.alive = False
                return
            # hit mine - shells pass through
            # hit tank
            for tank in tanks:
                if tank.alive and tank.x == int(self.x) and tank.y == int(self.y):
                    tank.alive = False
                    self.alive = False
                    return

    def draw(self, screen):
        px = self.x * CELL_SIZE + CELL_SIZE // 2
        py = self.y * CELL_SIZE + CELL_SIZE // 2
        pygame.draw.circle(screen, (200, 30, 30), (px, py), self.radius)

# ----- DRAWING -----
def draw_grid(screen):
    for y in range(GRID_HEIGHT):
        for x in range(GRID_WIDTH):
            rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE)
            pygame.draw.rect(screen, (230, 230, 230), rect)
            pygame.draw.rect(screen, (100, 100, 100), rect, 1)


def draw_map(screen, images):
    for y, row in enumerate(grid):
        for x, cell in enumerate(row):
            rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE)
            if cell == '#':
                screen.blit(images['wall'], rect)
            elif cell == '@':
                screen.blit(images['mine'], rect)
            else:
                screen.blit(images['empty'], rect)


def draw_tanks(screen, tanks):
    for tank in tanks:
        if tank.alive:
            sprite = tank.get_sprite()
            if sprite:
                screen.blit(sprite, (tank.x * CELL_SIZE, tank.y * CELL_SIZE))


def draw_shells(screen, shells):
    for shell in shells:
        if shell.alive:
            shell.draw(screen)

# ----- HELPERS -----
def clean_action(token):
    return re.sub(r'\s*\(.*?\)', '', token).strip() or "DoNothing"


def parse_action(action_str, tank):
    if not tank.alive:
        return None
    action = action_str.strip().lower()
    if action == "killed":
        tank.alive = False
    elif action == "moveforward":
        tank.move_forward()
    elif action == "movebackward":
        tank.move_backward()
    elif action == "rotateleft45":
        tank.rotate_left45()
    elif action == "rotateright45":
        tank.rotate_right45()
    elif action == "rotateleft90":
        tank.rotate_left90()
    elif action == "rotateright90":
        tank.rotate_right90()
    elif action == "shoot":
        return tank.shoot()
    return None

# ----- LOADING -----
def load_images():
    names = [
        'player1_up', 'player1_down', 'player1_left', 'player1_right',
        'player1_upright', 'player1_upleft', 'player1_downright', 'player1_downleft',
        'player2_up', 'player2_down', 'player2_left', 'player2_right',
        'player2_upright', 'player2_upleft', 'player2_downright', 'player2_downleft',
        'wall', 'mine', 'empty'
    ]
    images = {}
    for name in names:
        path = f'images/{name}.png'
        img = pygame.image.load(path).convert_alpha()
        images[name] = pygame.transform.scale(img, (CELL_SIZE, CELL_SIZE))
    return images


def load_map_dimensions():
    global GRID_WIDTH, GRID_HEIGHT
    with open("map.txt", "r") as f:
        lines = [line.strip() for line in f if line.strip()]
    for line in lines[:5]:
        if line.startswith("Rows"):
            GRID_HEIGHT = int(line.split('=')[1])
        elif line.startswith("Cols"):
            GRID_WIDTH = int(line.split('=')[1])


def load_map():
    global grid, wall_hits
    with open("map.txt", "r") as f:
        lines = f.readlines()[5:]
    grid = []
    wall_hits.clear()
    for y in range(GRID_HEIGHT):
        line = lines[y].rstrip("\n") if y < len(lines) else ""
        row = list((line + " " * GRID_WIDTH)[:GRID_WIDTH])
        grid.append(row)

# ----- MAIN LOOP -----
def main():
    pygame.init()
    load_map_dimensions()
    load_map()

    screen = pygame.display.set_mode((GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE))
    pygame.display.set_caption("Tank Game Visualizer")
    clock = pygame.time.Clock()
    images = load_images()

    # Init tanks
    tanks = []
    counts = {1: 0, 2: 0}
    for y in range(GRID_HEIGHT):
        for x in range(GRID_WIDTH):
            cell = grid[y][x]
            if cell.isdigit():
                num = int(cell)
                player_id = 1 if num % 2 == 1 else 2
                tanks.append(Tank(player_id, counts[player_id], x, y, images))
                counts[player_id] += 1
                grid[y][x] = '.'  # Clear tank digit

    # Load actions
    action_log = []
    with open("output.txt", "r") as f:
        for line in f:
            if line.strip() and not line.lower().startswith("player"):
                actions = [clean_action(t) for t in line.split(",")]
                action_log.append(actions)

    current_round = 0
    shells = []
    running = True

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Apply actions
        if current_round < len(action_log):
            actions = action_log[current_round]
            for i, tank in enumerate(tanks):
                act = actions[i] if i < len(actions) else "DoNothing"
                shell = parse_action(act, tank)
                if shell:
                    shells.append(shell)
            current_round += 1

        # Update shells
        # Move and handle collisions
        for shell in shells:
            if shell.alive:
                shell.update(tanks)

        # Shell-shell collisions
        positions = {}
        for shell in shells:
            if shell.alive:
                pos = (int(shell.x), int(shell.y))
                positions.setdefault(pos, []).append(shell)
        for pos, group in positions.items():
            if len(group) > 1:
                for sh in group:
                    sh.alive = False

        # Remove dead shells
        shells = [sh for sh in shells if sh.alive]

        # Draw everything
        screen.fill((255, 255, 255))
        draw_grid(screen)
        draw_map(screen, images)
        draw_tanks(screen, tanks)
        draw_shells(screen, shells)
        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
