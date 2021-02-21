int /* gvar_004C4848 */ modulationSources[145][2];

void /* FUN_0044feec */ initModulationSources() {
    modulationSources[0][0] = -1;
    modulationSources[0][1] = -1;

    for (int i = 0; i < 16; i++) {
        modulationSources[i + 1][0] = 1;
        modulationSources[i + 1][1] = i;
    }

    for (int i = 0; i < 130; i++) {
        modulationSources[i + 17][0] = 0;
        modulationSources[i + 17][1] = i;
    }
}

int /* FUN_00450d90 */ limitVelocity(int velocity) {
    if (velocity < 1) {
        return 0;
    }

    if (velocity > 126) {
        velocity = 126;
    }

    return velocity;
}