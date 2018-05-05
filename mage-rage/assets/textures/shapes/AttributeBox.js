/**
 * 
 */
var image = {
  render: function(ctx, entity) {
    var x = entity._geometry.x2 - entity._geometry.x;
    var y = entity._geometry.y2 - entity._geometry.y;

    var width = entity.width();
    var height = entity.height();

    var curr = entity._bindDataObject[entity._bindDataCurrAttrProperty];
    var max = entity._bindDataObject[entity._bindDataMaxAttrProperty];

    var ratio = curr / max;

    // Draw background rect.
    ctx.fillStyle = entity._backgroundColor;
    ctx.fillRect(x, y, width, height);

    // Draw overlay rect.
    ctx.fillStyle = entity._colorOverlay;
    ctx.fillRect(x, y, ratio * width, height);
  }
};