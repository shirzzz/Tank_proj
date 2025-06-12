import pygame
import sys
import re
import math

# ----- CONFIG -----
CELL_SIZE = 60
FPS = 5  # Increased for smoother animation
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
        print(f"Tank {self.player_id} at ({self.x}, {self.y}) facing {self.direction} shoots shell to ({sx}, {sy}) with velocity ({dx}, {dy})")
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
        # Check if shell is on a mine before moving
        if 0 <= self.x < GRID_WIDTH and 0 <= self.y < GRID_HEIGHT:
            cell = grid[int(self.y)][int(self.x)]
            if cell == '@':
                print(f"Shell on mine at ({int(self.x)}, {int(self.y)})")
        
        # Move shell one step at a time (slower, more visible movement)
        self.x += self.dx * 0.5  # Move half a cell per update for smoother animation
        self.y += self.dy * 0.5
        
        if WRAP_EDGES:
            self.x %= GRID_WIDTH
            self.y %= GRID_HEIGHT
        
        # Check bounds - give some margin so shells are visible longer
        if not (-2 <= self.x < GRID_WIDTH + 2 and -2 <= self.y < GRID_HEIGHT + 2):
            self.alive = False
            print(f"Shell went out of bounds at ({self.x:.1f}, {self.y:.1f})")
            return
        
        # Only check collisions if shell is within the game grid
        if 0 <= self.x < GRID_WIDTH and 0 <= self.y < GRID_HEIGHT:
            cell = grid[int(self.y)][int(self.x)]
            # hit wall
            if cell == '#':
                pos = (int(self.x), int(self.y))
                wall_hits[pos] = wall_hits.get(pos, 0) + 1
                if wall_hits[pos] >= 2:
                    grid[int(self.y)][int(self.x)] = '.'
                    print(f"Shell destroyed wall at ({int(self.x)}, {int(self.y)})")
                self.alive = False
                print(f"Shell hit wall at ({int(self.x)}, {int(self.y)})")
                return
            # hit mine - shells pass through but we'll show them
            elif cell == '@':
                print(f"Shell passing through mine at ({int(self.x)}, {int(self.y)})")
            # hit tank
            for tank in tanks:
                if tank.alive and tank.x == int(self.x) and tank.y == int(self.y):
                    tank.alive = False
                    self.alive = False
                    print(f"Shell hit tank {tank.player_id} at ({tank.x}, {tank.y})")
                    return

    def draw(self, screen):
        px = int(self.x * CELL_SIZE + CELL_SIZE // 2)
        py = int(self.y * CELL_SIZE + CELL_SIZE // 2)
        
        # Check if shell is on a mine
        is_on_mine = False
        if 0 <= self.x < GRID_WIDTH and 0 <= self.y < GRID_HEIGHT:
            cell = grid[int(self.y)][int(self.x)]
            is_on_mine = (cell == '@')
        
        # Draw a larger, more visible shell with a bright color
        if is_on_mine:
            # Draw a pulsing effect when on mine
            pulse = abs(math.sin(pygame.time.get_ticks() * 0.005)) * 4
            # Draw a bright red shell when on mine
            pygame.draw.circle(screen, (255, 0, 0), (px, py), 10 + pulse)  # Red outer shell
            pygame.draw.circle(screen, (255, 255, 0), (px, py), 6 + pulse)  # Yellow middle shell
            pygame.draw.circle(screen, (255, 100, 0), (px, py), 3 + pulse)  # Orange inner shell
        else:
            pygame.draw.circle(screen, (255, 255, 0), (px, py), 8)  # Yellow outer shell
            pygame.draw.circle(screen, (255, 100, 0), (px, py), 4)  # Orange inner shell

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
    # If action contains (killed) or (ignored), don't execute it
    if '(killed)' in token or '(ignored)' in token or '(Ignored)' in token:
        return "DoNothing"
    # Otherwise, remove parentheses and return the clean action
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
                tank = Tank(player_id, counts[player_id], x, y, images)
                tanks.append(tank)
                counts[player_id] += 1
                print(f"Tank {player_id} created at ({x}, {y}) facing {tank.direction}")
                grid[y][x] = '.'  # Clear tank digit

    # Load actions
    action_log = []
    with open("output.txt", "r") as f:
        for line in f:
            line = line.strip()
            if line and not line.lower().startswith("player") and not line.lower().startswith("tie"):
                # Split the line into individual actions
                actions = [clean_action(t) for t in line.split(",")]
                # Each line contains multiple rounds of actions
                # We need to group them into pairs (one for each tank)
                for i in range(0, len(actions), 2):
                    if i + 1 < len(actions):
                        round_actions = [actions[i], actions[i + 1]]
                        action_log.append(round_actions)
                        print(f"Loaded round with actions: {round_actions}")
    


    current_round = 0
    shells = []
    running = True

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
        
        # Process one round per frame
        if current_round < len(action_log):
            actions = action_log[current_round]
            print(f"\nProcessing round {current_round + 1}/{len(action_log)}")
            for i, tank in enumerate(tanks):
                if i < len(actions):
                    act = actions[i]
                    print(f"Tank {i+1} action: '{act}'")
                    if act.lower() == "shoot":
                        print(f"Tank {i+1} (at {tank.x},{tank.y}) attempting to shoot - alive: {tank.alive}")
                    shell = parse_action(act, tank)
                    if shell:
                        shells.append(shell)
                        print(f"Shell created at ({shell.x:.1f}, {shell.y:.1f}) moving ({shell.dx}, {shell.dy})")
            current_round += 1
            # Add a delay between rounds
            pygame.time.wait(500)  # 500ms delay between rounds

        # Update shells
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
        
        # Show round info on screen
        font = pygame.font.Font(None, 36)
        text = font.render(f"Round: {current_round}/{len(action_log)} | Active Shells: {len(shells)}", True, (0, 0, 0))
        screen.blit(text, (10, 10))
        
        pygame.display.flip()
        clock.tick(FPS)

        # Auto-exit when all rounds are complete and no shells remain
        if current_round >= len(action_log) and len(shells) == 0:
            print(f"\nGame visualization complete! Processed {len(action_log)} rounds.")
            # Wait a moment before closing
            pygame.time.wait(2000)
            running = False

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()