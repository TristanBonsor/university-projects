/**
 * 
 */
var image = {
  render: function(ctx, entity) {
    var x = entity._geometry.x2 - entity._geometry.x;
    var y = entity._geometry.y2 - entity._geometry.y;

    var radius = Math.random() * 50;
    
    ctx.beginPath();
    ctx.lineWidth = 10;
    ctx.strokeStyle = '#' + ('00000'+(Math.random()*(1<<24)|0).toString(16)).slice(-6);
    ctx.arc(x, y, radius, 0, 2 * Math.PI, false);
    ctx.stroke();
  }
};