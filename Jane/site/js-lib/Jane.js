"use strict";var EventSource=Object.create(null);EventSource.Init=function(a){this.subscriptions=[];if("name" in a){this["name"]=a.name}return this};EventSource.Subscribe=function(c,b){if(this.subscriptions.indexOf(c)>=0){return null}var a={target:c,receiver:b};this.subscriptions.push(a);return a};EventSource.Unsubscribe=function(b){var a=this.subscriptions.indexOf(b);if(a>=0){this.subscriptions.splice(a,1)}};EventSource.PostEvent=function(d){for(var a=0,c=this.subscriptions.length;a<c;++a){var b=this.subscriptions[a];b.receiver.apply(b.target,[this,d])}};var Jane=function(){var a=Object.create(EventSource).Init({name:"Jane"});a.constants={__IDENTIFIER__:"__IDENTIFIER__",__HIGHTLIGHTED__:"__HIGHTLIGHTED__"};a.events={DATA_REFERENCE_ADDED:"DATA_REFERENCE_ADDED",DATA_POPULATED:"DATA_POPULATED",DATA_FLUSHED:"DATA_FLUSHED",DATA_CHANGED:"DATA_CHANGED",HIGHLIGHT_CHANGED:"HIGHLIGHT_CHANGED"};a.dataRefs={index:{},nodes:[],links:[],depth:0,AddNode:function(d,b){console.log("node - "+d);var c=this.nodes.length;var e={name:d,arrayIndex:c,depth:0,children:[],dataRef:b};this.nodes.push(e);this.index[d]=e;return e}};a.dataRefs.AddNode("Jane",null);a.AddDataReference=function(b){var f=this.dataRefs;var d=f.AddNode(b.name,b);var e=("source" in b)?b.source.name:"Jane";if(e in f.index){var c=f.index[e];d.parent=c;c.children.push(d);d.depth=c.depth+1;f.depth=Math.max(d.depth,f.depth);f.links.push({source:c.index,target:d.index})}this.PostEvent(a.events.DATA_REFERENCE_ADDED)};a.GetDataReference=function(b){if(b in this.dataRefs.index){return this.dataRefs.index[b].dataRef}return null};return a}();Jane.Utility=function(b){var a=Object.create(b);a.SortLexical=function(f,d,i,j){switch(i){case"integer":case"double":case"string":var e=Number(f);var c=Number(d);if((e==f.toString())&&(c==d.toString())){return j?(e-c):(c-e)}return j?f.localeCompare(d):d.localeCompare(f);break;case"temporal":var h=new Date(f).valueOf();var g=new Date(d).valueOf();return j?(h-g):(g-h);break}return 0};a.ObjectIsEmpty=function(c){return(Object.getOwnPropertyNames(c).length==0)};return a}(null);Jane.DataObject=function(b){var a=Object.create(b,{allowFlushForSubscription:{value:false,enumerable:true,writable:true}});a.Init=function(c){b.Init.call(this,c);this.metaData={fields:{},tags:{}};this.keyName=Jane.constants.__IDENTIFIER__;this["filter"]=("filter" in c)?c.filter:null;this["select"]=("select" in c)?c.select:null;this["transform"]=("transform" in c)?c.transform:null;this["sort"]=("sort" in c)?c.sort:null;if("allowFlushForSubscription" in c){this["allowFlushForSubscription"]=c.allowFlushForSubscription}Jane.AddDataReference(this);return this};a.CanSubscribe=function(g){if(!Jane.Utility.ObjectIsEmpty(g)){for(var c in g){if(g.hasOwnProperty(c)){for(var d=0,f=this.subscriptions.length;d<f;++d){var e=this.subscriptions[d];if(c in e.contract){return false}}}}if(this.HasData()&&this.cachedData.readOnly){if(this.allowFlushForSubscription){this.Flush()}else{return false}}}return true};a.Subscribe=function(f,e,d){if(this.CanSubscribe(d)){var c=EventSource.Subscribe.call(this,f,e);if(c!=null){c.contract=d;if(this.HasData()){e.apply(f,[Jane.events.DATA_POPULATED,this.cachedData])}}return c}return null};a.SubscribeReadOnly=function(d,c){return this.Subscribe(d,c,{})};a.HasData=function(){return("cachedData" in this)};a.GetData=function(){return(this.HasData())?this.cachedData.records:null};a.GetDataIsReadOnly=function(){if(this.HasData()){return this.cachedData.readOnly}for(var c=0,e=this.subscriptions.length;c<e;++c){var d=this.subscriptions[c];if(!Jane.Utility.ObjectIsEmpty(d.contract)){return false}}return true};a.Validate=function(){if(!this.HasMetaData()){return false}var c=this.GetMetaData();if(this.select!=null){for(var d=0,e=this.select.length;d<e;++d){var f=this.select[d];if(!(f in c.fields)){return false}}}if(this.sort!=null){for(var d=0,e=this.sort.length;d<e;++d){var f=this.sort[d].name;if(!(f in c.fields)){return false}}}return true};a.CopyData=function(d,o){if(this.Validate()){var e=[];for(var g=0,l=d.length;g<l;++g){var k=d[g];if((this.filter==null)||(this.filter.HandleRecord(k))){if(this.select==null){if(!o){k=Object.create(k)}}else{var m={};for(var f=0,n=this.select.length;f<n;++f){var p=this.select[f];m[p]=k[p]}k=m}if(this.transform!=null){k=this.transform.HandleRecord(k,o)}e.push(k)}}if(this.sort!=null){var q=this;var c=this.GetMetaData();var h=this.sort.length;var r=function(s,j){for(var v=0,w=q.sort.length;v<w;++v){var t=q.sort[v];var u=Jane.Utility.SortLexical(s[t.name],j[t.name],c.fields[t.name].type,t.asc);if(u!=0){return u}}return 0};e.sort(r)}return e}return null};a.PopulateDataResponse=function(c,d){var e=this.GetDataIsReadOnly();if((this.sort!=null)||(this.select!=null)||(this.filter!=null)||(this.transform!=null)||(!e)){c=this.CopyData(c,e)}if(c!=null){this.cachedData={records:c,readOnly:e,key:null,indexes:{}};this.PostEvent(d)}};a.PopulateData=function(){};a.Populate=function(){if(!this.HasData()){this.PopulateData()}};a.Flush=function(){if(this.HasData()){delete this.cachedData;this.PostEvent(Jane.events.DATA_FLUSHED)}};a.Refresh=function(){this.Flush();this.Populate()};a.HasMetaData=function(){return !(Jane.Utility.ObjectIsEmpty(this.metaData.fields))};a.GetMetaData=function(){return this.metaData};a.AddTagMetaData=function(c,d){if(!(c in this.metaData.tags)){this.metaData.tags[c]=[]}this.metaData.tags[c].push(d)};a.AddFieldMetaData=function(j,c,f,d){var h={fieldName:j,displayName:c,type:f};this.metaData.fields[c]=h;for(var e=0,g=d.length;e<g;++e){this.AddTagMetaData(d[e],c)}};a.ValidateMetaData=function(){var c=this.GetMetaData();if("primary key" in c.tags){this.keyName=c.tags["primary key"][0]}else{this.AddFieldMetaData(this.keyName,"Key","integer",["primary key"])}};a.GetKey=function(){var f=null;if(this.HasData()){f=this.cachedData.key;if(f==null){var h=this.keyName;if(h in this.metaData.fields){var d=this.cachedData.records;if(!(h in d[0])){for(var e=0,g=d.length;e<g;++e){d[e][h]=e}}f={};for(var e=0,g=d.length;e<g;++e){var c=d[e];f[c[h]]=e}}this.cachedData.key=f}}return f};a.GetIndex=function(k){var e=null;if(this.HasData()){if(k in this.cachedData.indexes){e=this.cachedData.indexes[k]}else{if(k in this.metaData.fields){var d=this.cachedData.records;if(!(k in d[0])){if(k==this.keyName){for(var f=0,g=d.length;f<g;++f){d[f][k]=f}}}e=[];for(var f=0,g=d.length;f<g;++f){var c=d[f];e.push({value:c[k],index:f})}var h=this.metaData.fields;var j=function(l,i){return Jane.Utility.SortLexical(l.value,i.value,h[k].type,true)};e.sort(j);this.cachedData.indexes[k]=e}}}return e};return a}(EventSource);Jane.DataObjectReference=function(b){var a=Object.create(b);a.Init=function(c){if("source" in c){this["source"]=c.source}b.Init.call(this,c);this.source.SubscribeReadOnly(this,this.ReceiveEvent);return this};a.ReceiveEvent=function(c,d){switch(d){case Jane.events.DATA_POPULATED:if("populateRequested" in this){delete this.populateRequested;this.PopulateDataResponse(c.GetData(),d)}break;case Jane.events.DATA_CHANGED:if(this.HasData()){this.PopulateDataResponse(c.GetData(),d)}break;case Jane.events.DATA_FLUSHED:if(this.HasData()){this.Flush()}break;default:break}};a.PopulateData=function(){var c=this.source.GetData();if(c!=null){this.PopulateDataResponse(c,Jane.events.DATA_POPULATED)}else{this.populateRequested=true;this.source.Populate()}};a.HasMetaData=function(){return(b.HasMetaData.call(this))?true:this.source.HasMetaData()};a.GetMetaData=function(){return(b.HasMetaData.call(this))?b.GetMetaData.call(this):this.source.GetMetaData()};return a}(Jane.DataObject);Jane.DataObjectEspace=function(b){var a=Object.create(b);a.Init=function(c){if("resultSetUrl" in c){this["dataUrl"]=c.resultSetUrl}if("cdmMapUrl" in c){this["metaDataUrl"]=c.cdmMapUrl}if("dataSourceName" in c){this["metaDataName"]=c.dataSourceName}if("numRows" in c){this["recordCount"]=c.numRows}c.name=c.resultSetName;b.Init.call(this,c);this.PopulateMetaData();return this};a.PopulateMetaData=function(){if(!this.HasMetaData()){var e=document.createElement("a");e.href=this.dataUrl;var d=e.protocol+"//"+e.host+"/espace/rest/data/sources/"+this.metaDataName;var c=this;$.getJSON(d,function(f){c.PopulateMetaDataResponse(f)})}};a.PopulateMetaDataResponse=function(f){this.espaceMetaData=f;var c=f.columns;for(var d=0,e=c.length;d<e;++d){var g=c[d];this.AddFieldMetaData(g.accessName,g.displayName,g.types[0],g.tags)}this.ValidateMetaData();if("populateRequested" in this){delete this.populateRequested;this.PopulateData()}};a.PopulateEspaceFields=function(q){var l=this.espaceMetaData;var o=l.geoColumns;for(var g=0,k=o.length;g<k;++g){var d=o[g];var h=function(i){for(var j=0,s=l.columns.length;j<s;++j){var r=l.columns[j];if(r.accessName==i){return r.displayName}}};var n=h(d.geoColumn);var p=h(d.lonColumn);var c=h(d.latColumn);for(var f=0,m=q.length;f<m;++f){var e=q[f].data;e[n]={longitude:e[p],latitude:e[c]}}}};a.PopulateData=function(){if(this.HasMetaData()){var c=this;$.getJSON(this.dataUrl,function(d){c.PopulateEspaceFields(d.rows);c.PopulateDataResponse(d.rows,Jane.events.DATA_POPULATED)})}else{this.populateRequested=true}};return a}(Jane.DataObject);Jane.TransformFlatten=function(b){var a=Object.create(b,{name:{value:"Flatten",enumerable:true,writable:false}});a.EnumerateRecord=function(c,d){for(var e in c){if(c.hasOwnProperty(e)){var f=c[e];var g=typeof(f);if(g=="object"){this.EnumerateRecord(f,d)}else{d[e]=f}}}};a.HandleRecord=function(c,e){var d=Object.create(null);this.EnumerateRecord(c,d);return d};return a}(null);Jane.TransformExtract=function(b){var a=Object.create(b,{name:{value:"Extract",enumerable:true,writable:false},extract:{value:"xxx",enumerable:true,writable:true}});a.Init=function(c){if("extract" in c){this["extract"]=c.extract}return this};a.HandleRecord=function(c,d){if(this.extract in c){return d?c[this.extract]:Object.create(c[this.extract])}else{}return c};return a}(null);Jane.TransformCompound=function(b){var a=Object.create(b,{name:{value:"Compound",enumerable:true,writable:false}});a.Init=function(c){if("transforms" in c){this["transforms"]=c.transforms}return this};a.HandleRecord=function(c,f){for(var d=0,e=this.transforms.length;d<e;++d){c=this.transforms[d].HandleRecord(c,f)}return c};return a}(null);