# Git Workflow Guide for STM32 Project

## Quick Start

### First Time Setup
```bash
# Navigate to project directory
cd path/to/STM32

# Initialize git repository
git init

# Add all project files
git add .

# Create initial commit
git commit -m "Initial commit: STM32 ADC/DMA Timer project with UART interface"

# Add remote repository (replace with your repo URL)
git remote add origin https://github.com/your-username/STM32.git

# Rename branch to main
git branch -M main

# Push to remote
git push -u origin main
```

### Clone Existing Repository
```bash
git clone https://github.com/your-username/STM32.git
cd STM32
platformio run --target upload
```

## Daily Workflow

### Check Project Status
```bash
# See what files have changed
git status

# View specific changes
git diff

# View diff for specific file
git diff src/main.c
```

### Commit Changes
```bash
# Stage specific file
git add src/main.c

# Or stage all changes
git add .

# Commit with descriptive message
git commit -m "Add ADC calibration feature

- Implement 10-point ADC calibration
- Store calibration values in EEPROM
- Add calibration command via UART"
```

### Push and Pull
```bash
# Push commits to remote
git push origin main

# Pull latest changes
git pull origin main

# Fetch without merging
git fetch origin
```

## Feature Development Workflow

### Create Feature Branch
```bash
# Create new branch for your feature
git checkout -b feature/temperature-sensor

# Make changes and commit
git add .
git commit -m "Add temperature sensor integration"

# Push feature branch
git push -u origin feature/temperature-sensor
```

### Submit Pull Request
1. Push feature branch to GitHub
2. Create Pull Request on GitHub
3. Request code review
4. Address feedback
5. Merge to main after approval

### Update Feature Branch from Main
```bash
# Switch to main and pull latest
git checkout main
git pull origin main

# Switch back to feature branch
git checkout feature/temperature-sensor

# Rebase on latest main
git rebase main

# Force push to feature branch (use with caution)
git push -f origin feature/temperature-sensor
```

## Commit Message Best Practices

### Format
```
<type>: <subject>

<body>

<footer>
```

### Types
- `feat:` - New feature
- `fix:` - Bug fix
- `docs:` - Documentation changes
- `style:` - Code style changes (formatting, missing semicolons, etc.)
- `refactor:` - Code refactoring without feature changes
- `perf:` - Performance improvements
- `test:` - Adding/updating tests
- `chore:` - Build process, dependencies, etc.

### Examples
```bash
git commit -m "feat: Add DMA-based ADC sampling

Implement high-speed ADC sampling using DMA to reduce CPU overhead.
Sample rate increased from 100Hz to 1kHz.

Closes #42"
```

```bash
git commit -m "fix: Correct UART baud rate calculation

Fixed BRR register value for 115200 baud @ 16MHz HSI.
Previous value was causing data corruption.

Fixes #38"
```

## Useful Git Commands

### View History
```bash
# View commit log
git log

# View last 5 commits
git log -5

# View commits with file changes
git log --stat

# View commits in one line
git log --oneline
```

### Undoing Changes
```bash
# Undo uncommitted changes in specific file
git checkout -- src/main.c

# Undo all uncommitted changes
git checkout -- .

# Undo last commit (keep changes)
git reset HEAD~1

# Undo last commit (discard changes)
git reset --hard HEAD~1

# Revert specific commit (creates new commit)
git revert <commit-hash>
```

### Stashing
```bash
# Temporarily save uncommitted changes
git stash

# List all stashes
git stash list

# Apply most recent stash
git stash pop

# Apply specific stash
git stash apply stash@{0}

# Delete stash
git stash drop stash@{0}
```

### Tagging
```bash
# Create annotated tag
git tag -a v1.0 -m "Release version 1.0"

# Create lightweight tag
git tag v1.0

# View all tags
git tag

# Push tags to remote
git push origin --tags

# Push specific tag
git push origin v1.0
```

### Branching
```bash
# List all branches
git branch -a

# Create new branch
git branch feature/new-feature

# Switch branch
git checkout feature/new-feature

# Create and switch to new branch
git checkout -b feature/new-feature

# Delete branch locally
git branch -d feature/new-feature

# Delete branch on remote
git push origin --delete feature/new-feature

# Rename branch
git branch -m old-name new-name
```

## File Management

### `.gitignore` - What Gets Tracked
The project's `.gitignore` automatically excludes:
- **Build artifacts** - `.pio/`, `build/`, `*.o`, `*.elf`
- **IDE files** - `.vscode/`, `.idea/`, `*.sublime-*`
- **Temporary files** - `*.tmp`, `*.bak`, `*.swp`
- **Firmware** - `*.hex`, `*.bin`, `*.map`
- **Python cache** - `__pycache__/`, `*.pyc`

### `.gitattributes` - Line Endings
Ensures consistent line endings across platforms:
- **Unix (LF)** - Source code, scripts, documentation
- **Windows (CRLF)** - Batch files, PowerShell scripts

## Remote Repository Management

### Add Remote
```bash
git remote add origin <url>
```

### View Remotes
```bash
git remote -v
```

### Change Remote
```bash
git remote set-url origin <new-url>
```

### Remove Remote
```bash
git remote remove origin
```

## Troubleshooting

### Accidentally Committed Sensitive Information
```bash
# Remove file from all history
git filter-branch --tree-filter 'rm -f <file>' HEAD

# Force push (be careful!)
git push -f origin main
```

### Merge Conflicts
```bash
# View conflicts
git diff

# Edit conflicted files manually

# Mark as resolved
git add <file>

# Complete merge
git commit -m "Resolve merge conflicts"
```

### Undo Pushed Commits
```bash
# Revert specific commit
git revert <commit-hash>
git push origin main

# Or reset and force push (dangerous!)
git reset --hard HEAD~1
git push -f origin main
```

### Accidentally Deleted Branch
```bash
# Recover recent commits (30 days default)
git reflog
git checkout -b recovered-branch <commit-hash>
```

## Integration with PlatformIO

### Before Uploading Code
```bash
# Build project
platformio run -e blackpill_f411ce

# Check for uncommitted changes
git status

# Stage and commit changes
git add .
git commit -m "Code changes before upload"
```

### After Testing
```bash
# If changes work, push to remote
git push origin main

# If changes need revision
git add .
git commit -m "Bug fixes after testing"
```

## Team Collaboration

### Code Review Process
1. Create feature branch
2. Push branch and submit pull request
3. Wait for review comments
4. Address review feedback
5. Push updates
6. Merge when approved

### Keeping Fork Updated
```bash
# Add upstream remote
git remote add upstream https://github.com/original/repo.git

# Fetch upstream changes
git fetch upstream

# Rebase your branch
git rebase upstream/main

# Push updated branch
git push origin main
```

## Resources

- [Git Official Documentation](https://git-scm.com/doc)
- [GitHub Guides](https://guides.github.com/)
- [Pro Git Book](https://git-scm.com/book/en/v2)
- [Atlassian Git Tutorials](https://www.atlassian.com/git/tutorials)
