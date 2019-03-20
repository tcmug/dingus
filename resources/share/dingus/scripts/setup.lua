
-- Set up some common things
model = MODEL({
    file: 'monkey.obj'
})
monkey_edges = MODEL_BLOBIFY(model)
shader = SHADER('flat.vert', nil, 'flat.frag')
edges = SHADER('edges.vert', nil, 'edges.frag')

object = OBJECT({
    x = 0,
    y = 0,
    z = 0,
    model = monkey,
    shader = flat,
    kids = {
        OBJECT({
            model = monkey_edges,
            shader = edges
        })
    }
})



