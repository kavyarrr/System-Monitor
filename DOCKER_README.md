# Running System Monitor in Docker

## Prerequisites

- Docker installed on your system
- Docker Compose (optional, but recommended)

## Quick Start

### Method 1: Using Docker directly

1. **Build the Docker image:**
   ```bash
   docker build -t system-monitor .
   ```

2. **Run the container:**
   ```bash
   docker run -it --privileged system-monitor
   ```

   The `--privileged` flag is needed to access the `/proc` filesystem from inside the container.

### Method 2: Using Docker Compose (Recommended)

1. **Build and run:**
   ```bash
   docker-compose up --build
   ```

2. **To stop:**
   ```bash
   docker-compose down
   ```

## Important Notes

### Why `--privileged` is needed

The System Monitor reads from `/proc` filesystem which requires special permissions. The `--privileged` flag gives the container access to the host's `/proc` directory.

### Accessing Host's /proc

If you want the monitor to show the **host system's** processes (not container's), you can mount the host's `/proc`:

```bash
docker run -it --privileged -v /proc:/host/proc:ro system-monitor
```

Note: This requires modifying the code to read from `/host/proc` instead of `/proc`.

### Interactive Mode

The container runs in interactive mode (`-it`) so you can:
- See the ncurses UI properly
- Use Ctrl+C to exit
- Interact with the terminal

### Troubleshooting

1. **If you get "permission denied" errors:**
   - Make sure you're using `--privileged` flag
   - On Linux, you might need `sudo` for Docker commands

2. **If the UI doesn't display properly:**
   - Make sure your terminal supports TTY (interactive mode)
   - The `-it` flags are essential

3. **To rebuild after code changes:**
   ```bash
   docker build -t system-monitor .
   # or with docker-compose
   docker-compose build
   ```

4. **To run with a shell instead of the monitor:**
   ```bash
   docker run -it --privileged --entrypoint /bin/bash system-monitor
   ```

## File Structure

```
System-Monitor/
├── Dockerfile              # Docker build instructions
├── docker-compose.yml      # Docker Compose configuration
├── .dockerignore          # Files to exclude from Docker build
├── CMakeLists.txt         # CMake configuration
├── Makefile               # Make configuration
├── src/                   # Source files
├── include/               # Header files
└── build/                 # Build output (excluded from Docker)
```

## Development Workflow

1. Make code changes locally
2. Rebuild the Docker image:
   ```bash
   docker build -t system-monitor .
   ```
3. Run the updated container:
   ```bash
   docker run -it --privileged system-monitor
   ```

## Presentation Tips

When demonstrating in Docker:

1. **Show the Dockerfile:**
   - Explain the base image (Ubuntu)
   - Show dependency installation (build tools, ncurses)
   - Explain the build process

2. **Show the docker-compose.yml:**
   - Explain why `privileged: true` is needed
   - Show volume mounting for /proc (optional)

3. **Demonstrate:**
   ```bash
   # Build
   docker build -t system-monitor .
   
   # Run
   docker run -it --privileged system-monitor
   ```

4. **Explain benefits:**
   - Consistent environment across different machines
   - No need to install dependencies on host
   - Easy to share and deploy
   - Isolation from host system

