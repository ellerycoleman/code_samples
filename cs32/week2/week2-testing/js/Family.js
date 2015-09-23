
var Family = function(){
	// Initialize
	this.family = JSON.parse(sessionStorage.getItem('family')) || [];
    this.listenAddPerson();

	this.updateList();
};


Family.prototype.listenAddPerson = function()
{   var that= this;
	$('#addPersonForm').on('submit', 
	  function(event)
	  {   that.addPerson($('#person').val());
		  $('#person').val('');
		   return false;
      }
	 );
};

Family.prototype.addPerson = function(person)
{   this.family.push(person);
	this.updateList();
};

Family.prototype.removePerson = function(index)
{   console.log("removePerson() was called with index = '", index, "'");
    this.family.splice(index,1);
	this.updateList();
};

Family.prototype.updateList = function()
{   sessionStorage.setItem('family',JSON.stringify(this.family));
	this.updateFamilyDom();
}

Family.prototype.updateFamilyDom = function(){
    var that= this;
	var familyDom = this.family.map(
		function(person,index)
		{   console.log(  "updateFamilyDom.familyDom() was called with"
		                + " person='" + person + "'"
					    + " index='"  + index  + "'"
					   );	
			var removeButton = document.createElement("button");
		    removeButton.appendChild(document.createTextNode("remove"));
		    $(removeButton).click(function(){ that.removePerson(index); });
		    $(removeButton).addClass("btn");

		    var li = document.createElement('li');
		    li.appendChild(document.createTextNode(person));
		    li.appendChild(removeButton);
		    return li;
	    }
	);

	$('#currentList').html(familyDom);

};
