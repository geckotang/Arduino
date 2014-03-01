(function(){
  var sayTimeForm;
  var sayTimeBtn;
  var sayTextForm;
  var sayTextBtn;
  window.isNotBusy = function () {
    sayTextBtn.disabled = false;
    sayTimeBtn.disabled = false;
  }
  function load() {
    sayTextForm = document.getElementById('sayTextForm');
    sayTextBtn  = document.getElementById('sayTextBtn');
    sayTimeForm = document.getElementById('sayTimeForm');
    sayTimeBtn  = document.getElementById('sayTimeBtn');

    sayTextBtn.addEventListener("click", function(){
      this.disabled = true;
      sayTimeBtn.disabled = true;
      sayTextForm.submit();
    }, false);

    sayTimeBtn.addEventListener("click", function(){
      this.disabled = true;
      sayTextBtn.disabled = true;
      sayTimeForm.submit();
    }, false);

  }
	document.addEventListener("DOMContentLoaded", load, false);
})();
