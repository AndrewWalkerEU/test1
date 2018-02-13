function DigitalProductId(hexadecimalStringOrByteArray)
{
  if (typeof hexadecimalStringOrByteArray == "string")
  {
    hexadecimalStringOrByteArray = cvtHexStringToByteArray(hexadecimalStringOrByteArray);
  }
  if (hexadecimalStringOrByteArray != null)
  {
    if (hexadecimalStringOrByteArray.length >= 52 + 15)
    {
      hexadecimalStringOrByteArray = hexadecimalStringOrByteArray.slice(52);
    }
    hexadecimalStringOrByteArray = hexadecimalStringOrByteArray.slice(0, 15);
    if (hexadecimalStringOrByteArray.length < 15) hexadecimalStringOrByteArray = null;
  }
  this.byteArray = hexadecimalStringOrByteArray;
}

DigitalProductId.prototype.isValid = function()
{
  return this.byteArray != null;
}

DigitalProductId.prototype.toByteArray = function()
{
  var clonedArray = new Array(this.byteArray.length);
  var i;
  for (i = 0; i < this.byteArray.length; i++) clonedArray[i] = this.byteArray[i];
  return clonedArray;
}

DigitalProductId.prototype.toString = function()
{
  return cvtByteArrayToHexString(this.byteArray);
}

function ProductKey(keyString)
{
  this.keyString = keyString;
}

ProductKey.prototype.productKeyChars = ['B','C','D','F','G','H','J','K','M','P','Q','R','T','V','W','X','Y','2','3','4','6','7','8','9'];

ProductKey.prototype.loadFromDigitalProductId = function(digitalProductId)
{
  var digitalProductIdBytes = digitalProductId.toByteArray();
  var productKeyCharIndex, digitalProductIdBytesIndex;
  var keyString = "";
  for (productKeyCharIndex = this.productKeyChars.length; productKeyCharIndex >= 0; productKeyCharIndex--)
  {
    var accum = 0;
    for (digitalProductIdBytesIndex = 14; digitalProductIdBytesIndex >= 0; digitalProductIdBytesIndex--)
    {
      accum <<= 8;
      accum ^= digitalProductIdBytes[digitalProductIdBytesIndex];
      digitalProductIdBytes[digitalProductIdBytesIndex] = Math.floor(accum / 24);
      accum %= 24;
    }
    keyString = this.productKeyChars[accum] + keyString;
    if (productKeyCharIndex > 0 && productKeyCharIndex % 5 == 0) keyString = "-" + keyString;
  }
  this.keyString = keyString;
}

ProductKey.prototype.toString = function()
{
  return this.keyString;
}

function cvtHexStringToByteArray(hexString)
{
  var hexNormalized = hexString.replace(/\s*/g, "").toUpperCase();
  var byteArray = null;
  var i;
  if (hexNormalized.length % 2 == 0)
  {
    byteArray = new Array;
    for (i = 0; i < hexNormalized.length; )
    {
      var byteVal = parseInt(hexNormalized.charAt(i++) + hexNormalized.charAt(i++), 16);
      if (isNaN(byteVal))
      {
        byteArray = null;
    break;
      }
      byteArray.push(byteVal);
    }
  }
  return byteArray;
}

function cvtByteArrayToHexString(byteArray)
{
  var i, s = new Array;
  for (i = 0; i < byteArray.length; i++)
  {
    var hex = byteArray[i].toString(16);
    if (s.length > 0) s.push(" ");
    if (hex.length < 2) s.push("0");
    s.push(hex);
  }
  return s.join("").toUpperCase();
}
--></SCRIPT> <SCRIPT><!--
function handleDecodeButtonClick()
{
  var form = document.forms.decoderForm;
  var digitalProductId = new DigitalProductId(form.digitalProductIdHex.value);
  if (!digitalProductId.isValid())
  {
    form.productKey.value = "(Digital Product ID is invalid!)";
  }
  else
  {
    var productKey = new ProductKey;
    productKey.loadFromDigitalProductId(digitalProductId);
    document.getElementBy
    setProductKeyValueText(productKey.keyString);
    setAssociatedInfoValueText(form.associatedInfo.value);
  }
  return false;
}

function handleSectionDisplayClick(linkElement, elementId)
{
  var element = document.getElementById(elementId);
  if (element != null)
  {
    if (element.style.display == "none")
    {
      element.style.display = "block";
      linkElement.firstChild.nodeValue = "hide";
    }
    else
    {
      element.style.display = "none";
      linkElement.firstChild.nodeValue = "show";
    }
  }
  return false;
}

function setProductKeyValueText(text)
{
  document.getElementById("productKeyId").firstChild.nodeValue = text;
}

function setAssociatedInfoValueText(text)
{
  text = text.replace(/\r\n/g, "\r");
  document.getElementById("associatedInfoId").firstChild.nodeValue = text;
  document.getElementById("associatedInfoDivId").style.display =
    text.replace(/\s*/g, "") == "" ? "none" : "block";
}
