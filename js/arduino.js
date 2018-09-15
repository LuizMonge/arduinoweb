$(function(){
    $('.icone').hover(function(){
      $(this).animate({
        'width': 200,
        'height': 120,
        'margin-top': -30},200);
      },function(){
        $(this).animate({
          'width': 120,
          'height': 120,
          'margin-top': 10},300);
      });
});
