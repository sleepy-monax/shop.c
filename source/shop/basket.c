#include <stdlib.h>
#include <string.h>

#include "model/view.h"
#include "shop/basket.h"
#include "utils/assert.h"
#include "utils/math.h"
#include "utils/terminal.h"

Basket *basket_create(StockList *stocks, Client *owner) {
  Basket *this = malloc(sizeof(Basket));

  this->pay_with_point = false;
  this->items = list_create();
  this->stocks = stocks;
  this->owner = owner;

  return this;
}

void basket_destroy(Basket *this) {
  list_destroy(this->items);
  free(this);
}

BasketItem *Basket_lookup(Basket *this, BareCode id, bool is_consigned) {
  list_foreach(item, this->items) {
    BasketItem *b = (BasketItem *)item->value;

    if (b->barecode == id &&
        (!is_consigned || b->is_consigne == is_consigned)) {
      return b;
    }
  }
  return NULL;
}

void basket_add_item(Basket *this, BareCode barecode, bool is_consigned,
                     int quantity) {
  BasketItem *existingItem;
  existingItem = Basket_lookup(this, barecode, is_consigned);

  if (existingItem != NULL)
    existingItem->quantity += quantity;
  else {
    BasketItem *item = malloc(sizeof(BasketItem));

    item->barecode = barecode;
    item->quantity = quantity;
    item->is_consigne = is_consigned;

    list_pushback(this->items, item);
  }
}

float basket_pay(User *user, Basket *this, FILE *fout) {
  printf("Votre caissier: %s.\n\n", user->lastname);

  if (fout) {
    fprintf(fout, "Voici le contenu du panier : \n\n");
    fprintf(fout, " N°.art | Dénomination               | Vidange | Qte | Prix "
                  "unitaire | Montant \n");
    fprintf(fout, "------------------------------------------------------------"
                  "-------------------\n");
  }

  float basket_total = 0;
  float basket_discount = 0;

  list_foreach(item, this->items) {
    BasketItem *basket_item = (BasketItem *)item->value;
    Item *stock_item = stocks_lookup_item(this->stocks, basket_item->barecode);

    fprintf(fout, "  %04d  |", basket_item->barecode);
    fprintf(fout, " %-26s |", stock_item->label);

    if (basket_item->is_consigne) {
      fprintf(fout, " oui     |");
    } else {
      fprintf(fout, "         |");
    }

    if (basket_item->quantity) {
      fprintf(fout, " x%-2d |", basket_item->quantity);
    } else {
      fprintf(fout, "      ");
    }

    float item_unit_value = 0.0;
    float item_unit_discount = 0.0;

    if (basket_item->is_consigne) {
      item_unit_value = -stock_item->consignedValue;
      fprintf(fout, " %5.2f         |", item_unit_value);
    } else {
      item_unit_value = stock_item->price;
      item_unit_discount = item_unit_value * (stock_item->discount / 100.0);

      if (stock_item->discount) {
        fprintf(fout, " %5.2f  -%-2d%%   |", item_unit_value,
                stock_item->discount);
      } else {
        fprintf(fout, " %5.2f         |", item_unit_value);
      }
    }

    fprintf(fout, " %7.2f \n", item_unit_value * basket_item->quantity);

    basket_total += item_unit_value * basket_item->quantity;
    basket_discount += item_unit_discount * basket_item->quantity;
  }

  fprintf(fout, "\n------------------------------------------------------------"
                "-------------------\n\n");

  float basket_point_discount = 0.0;

  fprintf(fout, "Total hors réduction: %.2f€\n", basket_total);

  if (this->owner && this->pay_with_point) {
    int point_used =
        min((basket_total - basket_discount) * 100, this->owner->points);

    fprintf(fout, "Reduction fidelité: %.2f€ (%2dpts)\n", point_used / 100.0,
            point_used);

    basket_point_discount = (point_used / 100.0);
    this->owner->points -= point_used;
  }

  float basket_final_total =
      basket_total - basket_discount - basket_point_discount;

  fprintf(fout, "Réduction: %.2f€\n", basket_discount);
  fprintf(fout, "\nTotal à payer: %.2f€\n\n", basket_final_total);

  if (this->owner) {
    fprintf(fout, "Vous avez gagnez %dpts\n", (int)basket_final_total / 10);
    this->owner->points += basket_total / 10;
  }

  // Apply stocks changes.

  list_foreach(item, this->items) {
    BasketItem *item_in_basket = (BasketItem *)item->value;

    if (!item_in_basket->is_consigne) {
      stocks_lookup_item(this->stocks, item_in_basket->barecode)->quantity -=
          item_in_basket->quantity;
    }
  }

  return basket_total;
}

typedef enum {
  COL_BASKET_BARECODE,
  COL_BASKET_LABEL,
  COL_BASKET_CONSIGNE,
  CAL_BASKET_UNIT_PRICE,
  COL_BASKET_QUANTITY,
  COL_BASKET_REDUCTION,
  COL_BASKET_PRICE,

  __COL_BASKET_COUNT,
} BasketModelColumn;

ModelAccess basket_ModelReadAccess(Basket *basket, int row, int column,
                                   User *user) {
  (void)basket;
  (void)row;
  (void)column;
  (void)user;

  return ACCESS_ALL;
}

ModelAccess basket_ModelWriteAccess(Basket *basket, int row, int column,
                                    User *user) {
  (void)basket;
  (void)row;
  (void)user;

  if (column == COL_BASKET_CONSIGNE) {
    BasketItem *item_in_basket = NULL;
    list_peekat(basket->items, row, (void **)&item_in_basket);
    Item *item_in_stock =
        stocks_lookup_item(basket->stocks, item_in_basket->barecode);

    if (item_in_stock != NULL && item_in_stock->isConsigned) {
      return ACCESS_ALL;
    }

    return ACCESS_NONE;
  }

  if (column == COL_BASKET_BARECODE || column == COL_BASKET_QUANTITY ||
      column == COL_BASKET_CONSIGNE) {
    return ACCESS_ALL;
  } else {
    return ACCESS_NONE;
  }
}

int basket_ModelRowCount(Basket *basket) { return list_count(basket->items); }

int basket_ModelRowCreate(Basket *basket) {
  BasketItem *item = malloc(sizeof(BasketItem));

  *item = (BasketItem){0};

  list_pushback(basket->items, item);

  return list_count(basket->items) - 1;
}

void basket_ModelRowDelete(Basket *basket, int index) {
  BasketItem *item_to_remove;
  list_peekat(basket->items, index, (void **)&item_to_remove);
  list_remove(basket->items, item_to_remove);
}

int basket_ModelColumnCount(void) { return __COL_BASKET_COUNT; }

const char *basket_ModelColumnName(int index, ModelRole role) {
  (void)role;

  if (role == ROLE_DISPLAY) {
    switch (index) {
    case COL_BASKET_BARECODE:
      return "N°.art";

    case COL_BASKET_LABEL:
      return "Dénomination";

    case COL_BASKET_CONSIGNE:
      return "Vidange";

    case CAL_BASKET_UNIT_PRICE:
      return "Prix Unitaire";

    case COL_BASKET_QUANTITY:
      return "Quantitée";

    case COL_BASKET_REDUCTION:
      return "Réduction";

    case COL_BASKET_PRICE:
      return "Montant";
    }
  } else {

    switch (index) {
    case COL_BASKET_BARECODE:
      return "BARECODE";

    case COL_BASKET_LABEL:
      return "LABEL";

    case COL_BASKET_CONSIGNE:
      return "CONSIGNE";

    case CAL_BASKET_UNIT_PRICE:
      return "UNIT_PRICE";

    case COL_BASKET_QUANTITY:
      return "QUANTITY";

    case COL_BASKET_REDUCTION:
      return "REDUCTION";

    case COL_BASKET_PRICE:
      return "PRICE";
    }
  }

  ASSERT_NOT_REACHED();
}

VarianType basket_ModelColumnType(int index, ModelRole role) {
  (void)role;

  switch (index) {
  case COL_BASKET_BARECODE:
    return VARIANT_INT;

  case COL_BASKET_LABEL:
    return VARIANT_STRING;

  case COL_BASKET_CONSIGNE:
    return VARIANT_INT;

  case CAL_BASKET_UNIT_PRICE:
    return VARIANT_FLOAT;

  case COL_BASKET_QUANTITY:
    return VARIANT_INT;

  case COL_BASKET_REDUCTION:
    return VARIANT_FLOAT;

  case COL_BASKET_PRICE:
    return VARIANT_FLOAT;
  }

  ASSERT_NOT_REACHED();
}

Style basket_ModelColumnStyle(int index) {
  (void)index;

  return style_centered(DEFAULT_STYLE);
}

Variant basket_ModelGetData(Basket *basket, int row, int column,
                            ModelRole role) {
  (void)role;

  BasketItem *basket_item;
  list_peekat(basket->items, row, (void **)&basket_item);
  Item *item = stocks_lookup_item(basket->stocks, basket_item->barecode);

  if (item == NULL) {
    return vstring("(null)");
  } else {
    if (column == COL_BASKET_QUANTITY) {
      if (role == ROLE_DATA || role == ROLE_EDITOR) {
        return vint(basket_item->quantity);
      } else {
        return vstringf("x%-3d", basket_item->quantity);
      }
    }

    if (basket_item->is_consigne) {
      switch (column) {
      case COL_BASKET_BARECODE:
        return vint(basket_item->barecode);

      case COL_BASKET_LABEL:
        return vstring(item->label);

      case COL_BASKET_CONSIGNE:
        if (role == ROLE_DATA || role == ROLE_EDITOR) {
          return vint(1);
        } else {
          return vstring("oui");
        }

      case CAL_BASKET_UNIT_PRICE:
        if (role == ROLE_DATA || role == ROLE_EDITOR) {
          return vfloat(item->consignedValue);
        } else {
          return vstringf("%5.2f€", item->consignedValue);
        }

      case COL_BASKET_REDUCTION:
        return vstring("-");

      case COL_BASKET_PRICE:
        if (role == ROLE_DATA || role == ROLE_EDITOR) {
          return vfloat(-(item->consignedValue * basket_item->quantity));
        } else {
          return vstringf("-%5.2f€",
                          (item->consignedValue * basket_item->quantity));
        }
      }

      ASSERT_NOT_REACHED();
    } else {
      switch (column) {
      case COL_BASKET_BARECODE:
        return vint(basket_item->barecode);

      case COL_BASKET_LABEL:
        return vstring(item->label);

      case COL_BASKET_CONSIGNE:
        if (role == ROLE_DATA || role == ROLE_EDITOR) {
          return vint(0);
        } else {
          return vstring("non");
        }

      case CAL_BASKET_UNIT_PRICE:
        if (role == ROLE_DATA || role == ROLE_EDITOR) {
          return vfloat(item->price);
        } else {
          return vstringf("%5.2f€", item->price);
        }

      case COL_BASKET_REDUCTION:
        if (role == ROLE_DISPLAY) {
          if (item->discount) {
            return vstringf("-%2d%%", item->discount);
          } else {
            return vstring("-");
          }
        } else {
          return vint(item->discount);
        }

      case COL_BASKET_PRICE: {
        float total_value = (item->price * basket_item->quantity);

        if (role == ROLE_DATA || role == ROLE_EDITOR) {
          return vfloat(total_value);
        } else {
          return vstringf("%5.2f€", total_value);
        }
      }
      }

      ASSERT_NOT_REACHED();
    }
  }
}

void basket_ModelSetData(Basket *basket, int row, int column, Variant value,
                         ModelRole role) {
  (void)role;

  BasketItem *basket_item;
  list_peekat(basket->items, row, (void **)&basket_item);

  switch (column) {
  case COL_BASKET_BARECODE:
    basket_item->barecode = value.as_int;
    break;
  case COL_BASKET_CONSIGNE:
    basket_item->is_consigne = value.as_int;
    break;
  case COL_BASKET_QUANTITY:
    basket_item->quantity = value.as_int;
    break;
  }
}

ModelAction basket_actions[] = {
    DEFAULT_MODEL_VIEW_ACTION END_MODEL_VIEW_ACTION};

ModelAction *basket_ModelGetActions(void) { return basket_actions; }

Model basket_model_create(void) {
  return (Model){
      (ModelReadAccess)basket_ModelReadAccess,
      (ModelWriteAccess)basket_ModelWriteAccess,

      (ModelRowCount)basket_ModelRowCount,
      (ModelRowCreate)basket_ModelRowCreate,
      (ModelRowDelete)basket_ModelRowDelete,

      (ModelColumnCount)basket_ModelColumnCount,
      (ModelColumnName)basket_ModelColumnName,
      (ModelColumnType)basket_ModelColumnType,
      (ModelColumnStyle)basket_ModelColumnStyle,

      (ModelGetData)basket_ModelGetData,
      (ModelSetData)basket_ModelSetData,

      (ModelGetActions)basket_ModelGetActions,
  };
}