
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
{   console.log("addPerson() was call with " + arguments.length
		        + " args; person='" + person + "'");
	this.family.push(person);
	this.updateList();
};

Family.prototype.removePerson = function(index)
{   console.log("removePerson() was called with " + arguments.length  
		        + " args; index = '" + index + "'");
    this.family.splice(index,1);
	this.updateList();
};

Family.prototype.updateList = function()
{   console.log("updateList() called with " + arguments.length + " args");
	sessionStorage.setItem('family',JSON.stringify(this.family));
	this.updateFamilyDom();
}

Family.prototype.updateFamilyDom = function(){
	console.log("updateFamilyDom() called with " + arguments.length + " args");
    var that= this;
	var familyDom = this.family.map(
		function(person,index)
		{   console.log(  "updateFamilyDom.familyDom() was called with "
		                + arguments.length + " args; "
			            + " person='" + person + "'"
					    + " index='"  + index  + "'"
					   );	
		    var i;
			for(i=0; i<arguments.length; i++)
		    {   console.log("args[" + i + "]:" + arguments[i]);
            }
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
