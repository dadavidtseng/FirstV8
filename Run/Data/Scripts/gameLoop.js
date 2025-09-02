var shouldRender = true;

function update() {
    game.update(0.0, 0.0);
    if (input.wasKeyJustPressed(112))
    {
        shouldRender = !shouldRender;
    }
}

function render() {
    if (shouldRender) {
        game.render(0.0, 0.0);
    }
}