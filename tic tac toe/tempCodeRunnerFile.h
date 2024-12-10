
std::ostream& operator<<(std::ostream& os, const GameState& state) {
    os << "\n ";
    for (int j = 0; j < state.size; j++) {
        os << "   " << j;
    }
    os << "\n";
    
    for (int i = 0; i < state.size; i++) {
        os << "  +" << string(state.size * 4 - 1, '-') << "+\n";
        os << i << " |";
        for (int j = 0; j < state.size; j++) {
            char c = ' ';
            if (state.grid[i][j] == 0) {
                c = 'X';
            } else if (state.grid[i][j] == 1) {
                c = 'O';
            }
            os << " " << c << " |";
        }
        os << "\n";
    }
    os << "  +" << string(state.size * 4 - 1, '-') << "+\n";
    return os;
}


#endif

