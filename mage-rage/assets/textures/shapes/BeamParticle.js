var image = {
  render: function(ctx, entity) {
    switch(entity._beamType) {
      case 'air':
        ctx.fillStyle = 'rgba(255, 255, 255, 1)';
        break;
      case 'earth':
        ctx.fillStyle = 'rgba(85, 34, 0, 1)';
        break;
      case 'fire':
        ctx.fillStyle = 'rgba(255, 0, 0, 1)';
        break;
      case 'water':
        ctx.fillStyle = 'rgba(0, 0, 255, 1)';
        break;
      default:
        ctx.fillStyle = 'rgba(0, 255, 0, 1)';
        break;
    }

    var p1, p2, p3, p4;

    switch(entity._direction) {
      case 'up':
        p1 = new IgePoint(entity._geometry.x - 37, entity._geometry.y - 32, 0);
        p2 = new IgePoint(entity._geometry.x - 42, entity._geometry.y - 32, 0);
        p3 = new IgePoint(entity._geometry.x - 42, entity._geometry.y - 92, 0);
        p4 = new IgePoint(entity._geometry.x - 37, entity._geometry.y - 92, 0);
        break;
      case 'down':
        p1 = new IgePoint(entity._geometry.x - 37, entity._geometry.y - 32, 0);
        p2 = new IgePoint(entity._geometry.x - 42, entity._geometry.y - 32, 0);
        p3 = new IgePoint(entity._geometry.x - 42, entity._geometry.y + 28, 0);
        p4 = new IgePoint(entity._geometry.x - 37, entity._geometry.y + 28, 0);
        break;
      case 'left':
        p1 = new IgePoint(entity._geometry.x - 37, entity._geometry.y - 27, 0);
        p2 = new IgePoint(entity._geometry.x - 37, entity._geometry.y - 22, 0);
        p3 = new IgePoint(entity._geometry.x - 97, entity._geometry.y - 22, 0);
        p4 = new IgePoint(entity._geometry.x - 97, entity._geometry.y - 27, 0);
        break;
      case 'right':
        p1 = new IgePoint(entity._geometry.x - 37, entity._geometry.y - 27, 0);
        p2 = new IgePoint(entity._geometry.x - 37, entity._geometry.y - 22, 0);
        p3 = new IgePoint(entity._geometry.x + 23, entity._geometry.y - 22, 0);
        p4 = new IgePoint(entity._geometry.x + 23, entity._geometry.y - 27, 0);
        break;
      case 'upLeft':
        p1 = new IgePoint(entity._geometry.x - 32, entity._geometry.y - 28, 0);
        p2 = new IgePoint(entity._geometry.x - 29, entity._geometry.y - 32, 0);
        p3 = new IgePoint(entity._geometry.x - 70, entity._geometry.y - 74, 0);
        p4 = new IgePoint(entity._geometry.x - 73, entity._geometry.y - 70, 0);
        break;
      case 'downLeft':
        p1 = new IgePoint(entity._geometry.x - 32, entity._geometry.y - 28, 0);
        p2 = new IgePoint(entity._geometry.x - 35, entity._geometry.y - 32, 0);
        p3 = new IgePoint(entity._geometry.x - 73, entity._geometry.y + 6, 0);
        p4 = new IgePoint(entity._geometry.x - 70, entity._geometry.y + 10, 0);
        break;
      case 'upRight':
        p1 = new IgePoint(entity._geometry.x - 32, entity._geometry.y - 28, 0);
        p2 = new IgePoint(entity._geometry.x - 35, entity._geometry.y - 32, 0);
        p3 = new IgePoint(entity._geometry.x + 6, entity._geometry.y - 74, 0);
        p4 = new IgePoint(entity._geometry.x + 9, entity._geometry.y - 70, 0);
        break;
      case 'downRight':
        p1 = new IgePoint(entity._geometry.x - 32, entity._geometry.y - 28, 0);
        p2 = new IgePoint(entity._geometry.x - 29, entity._geometry.y - 32, 0);
        p3 = new IgePoint(entity._geometry.x + 9, entity._geometry.y + 6, 0);
        p4 = new IgePoint(entity._geometry.x + 6, entity._geometry.y + 10, 0);
        break;
      default:
        p1 = new IgePoint(entity._geometry.x - 32, entity._geometry.y - 32, 0);
        p2 = new IgePoint(entity._geometry.x - 32, entity._geometry.y - 27, 0);
        p3 = new IgePoint(entity._geometry.x - 27, entity._geometry.y - 27, 0);
        p4 = new IgePoint(entity._geometry.x - 27, entity._geometry.y - 32, 0);
        break;
    }

    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.lineTo(p3.x, p3.y);
    ctx.lineTo(p4.x, p4.y);
    ctx.closePath();
    ctx.fill();
  }
};