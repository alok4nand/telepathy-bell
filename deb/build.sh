# Script to generate and install telepathy-bell .deb pkg
# Run this from `deb` directory

# Build telepathy-bell .deb pkg
dpkg-buildpackage -us -uc -b

# Install telepathy-bell .deb pkg
# sudo dpkg -i ../telepathy-bell_1.0.0-0\~8.5_all.deb
